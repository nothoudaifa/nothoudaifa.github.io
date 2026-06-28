---
author: "nothoudaifa"
title: "Extensif chall from ECSC 2026 writeup"
date: "2026-06-26"
tags: 
- ctf 
- rev
- hardware
---

you can check the challenge [here](https://hackropole.fr/en/challenges/reverse/fcsc2026-reverse-extensif/), difficulty is 1 star.

## TL;DR
reversing an esp32 app image that does basic flag checking

## Solve
we are given a file `extensif.bin` running `file` on it gives us this:
```
$ file extensif.bin 
extensif.bin: ESP-IDF application image for ESP32, project name: "extensif", version 1, compiled on Mar 24 2026 15:29:16, IDF version: v5.5.1, entry address: 0x400811E8
```
it is not an elf, googling `ESP-IDF` i find this [github list](https://github.com/BlackVS/ESP32-reversing) which contains a lot of resources on esp32 reversing, i spent some time browsing it and trying different stuff until i came up against this [ghidra extension](https://github.com/Ebiroll/esp32_flash_loader), which is used to load esp32 flash dumps, using it i can load extensif.bin successfully into ghidra.

when first loading the binary i find my self in main which is not very helpfull since the main is not of the flag checker rather it is of whatever runs on esp32 when it boots (apparently it's freertos, i grepped for it inside the binary), so to find the user code, i opened Windows->Defined Strings and searched for the flag format `FCSC`.
i found this:
![FCSC String filter screenshot](/ecsc2026-extensif/2026-06-26|17:41:41.464.png)
getting the xrefs for this string i find my self at the function that appears to be the main user code
```c

void UndefinedFunction_400d6000(void)
{
  code *pcVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  char acStack_51 [33];
  byte abStack_30 [48];
  
  puVar2 = PTR_s_Bienvenue_400d0698;
  func_0x400d8590(puVar2);
  for (iVar5 = 0; iVar5 < 4; iVar5 = iVar5 + 1) {
    iVar4 = iVar5 * 4;
    puVar2 = PTR_DAT_400d069c;
    puVar2[iVar5 * 4] = 0x46;
    puVar2[iVar4 + 1] = 0x43;
    puVar2[iVar4 + 2] = 0x53;
    puVar2[iVar4 + 3] = 0x43;
  }
  func_0x400d60e4();
  for (iVar5 = 0; iVar5 < 0x10; iVar5 = iVar5 + 1) {
    puVar3 = PTR_BYTE_400d06a0;
    puVar2 = PTR_DAT_400d069c;
    abStack_30[iVar5] = puVar3[iVar5] ^ puVar2[iVar5];
  }
  puVar2 = PTR_s_Entrez_le_flag_(16_chars):_400d06a4;
  func_0x400d8438(puVar2);
  iVar5 = func_0x40088434();
  pcVar1 = (code *)PTR_fflush_400d06b8;
  (*pcVar1)(*(undefined4 *)(iVar5 + 8));
  iVar5 = func_0x40088434();
  iVar5 = func_0x400d8048(acStack_51 + 1,0x20,*(undefined4 *)(iVar5 + 4));
  if (iVar5 == 0) {
    puVar2 = PTR_s_Erreur_de_lecture_400d06a8; // -- (1)
    func_0x400d8590(puVar2);
  }
  else {
    pcVar1 = (code *)PTR_strlen_400d0660;
    iVar5 = (*pcVar1)(acStack_51 + 1);
    if (iVar5 != 0) {
      if (acStack_51[iVar5] == '\n') {
        acStack_51[iVar5] = '\0';
        iVar5 = iVar5 + -1;
      }
      if (iVar5 == 0x10) {
        pcVar1 = (code *)PTR_memcmp_400d067c;
        iVar5 = (*pcVar1)(acStack_51 + 1,abStack_30,0x10);
        if (iVar5 == 0) {
          func_0x400d8504(10);
          puVar2 = PTR_DAT_400d06b0;
          func_0x400d8438(puVar2,acStack_51 + 1);
          return;
        }
        puVar2 = PTR_s_[-]_Essaie_encore_400d06b4;
        func_0x400d8590(puVar2);
        return;
      }
    }
    puVar2 = PTR_s_[-]_Longueur_invalide._400d06ac;
    func_0x400d8590(puVar2);
  }
  return;
}
```

as you can see, it's pretty bad decompilation, one reason for this is that the architecture of the esp32 Xtensa uses 24 or 16 bit instructions sizes which means you need more than one instruction to assgin an address to registers, so what the compiler does is it puts addresses into nearby memory locations so they can be addressed relatively (got this from [here](https://vik0t0r.github.io/posts/ESP32-arduino-RE/#step-4-analysis)).

anyway, this program is reading a 0x10 long flag into `acStack_51 + 1` and memcmp'ing it with `abStack_30` so we just need to figure out what's in it, i used error messages to figure out what it's doing for example it's trying to print the message `PTR_s_Erreur_de_lecture_400d06a8` which obviously means the function before tries to read input.

the program tries to write the xor of PTR_BYTE_400d06a and PTR_DAT_400d069c into `abStack_30` in here:
```c
  for (iVar5 = 0; iVar5 < 0x10; iVar5 = iVar5 + 1) {
    puVar3 = PTR_BYTE_400d06a0;
    puVar2 = PTR_DAT_400d069c;
    abStack_30[iVar5] = puVar3[iVar5] ^ puVar2[iVar5];
  }
```
PTR_BYTE_400d06a0 is constant and PTR_DAT_400d069c is set just above it, first it's set to b'FCSCFCSCFCSCFCSC' using:
```
  for (iVar5 = 0; iVar5 < 4; iVar5 = iVar5 + 1) {
    iVar4 = iVar5 * 4;
    puVar2 = PTR_DAT_400d069c;
    puVar2[iVar5 * 4] = 0x46;
    puVar2[iVar4 + 1] = 0x43;
    puVar2[iVar4 + 2] = 0x53;
    puVar2[iVar4 + 3] = 0x43;
  }
```
then the first 12 bytes are overwritten by the call to `func_0x400d60e4`

inspecting `func_0x400d60e4`
we see it's decompilation and disassembly:
![func_0x400d60e4 decomp](/ecsc2026-extensif/2026-06-26|18:07:55.187.png)
it calls `func_0x400d60f4` with 0x4a and `PTR_DAT_400d069c`
here is the code for `func_0x400d60f4`
![func_0x400d60f4 decomp](/ecsc2026-extensif/2026-06-26|18:11:21.474.png)
it writes the first argument into the first byte of the second argument and it writes the first byte of the return address into the second byte and incremnts the first arg by one and second arg by 2 and keeps calling it self recursively until `arg1 & 8 == 0`

getting the return addresses is easy, the return address is the address of the instrution after the call instruction, which means:
for the first time when it's called by `func_0x400d60e4`
```assembly
      400d60ec 65 00 00      call8     SUB_400d60f4
here->400d60ef 1d f0         retw.n
```
we can see that it's 0xef
and for the rest it's 0x08
```assembly
      400d6105 e5 fe ff      call8     SUB_400d60f4
                         LAB_400d6108                              XREF[1]:   400d60f7(j)  
here->400d6108 1d f0         retw.n
```

now tracing the execution of this function we can deduce that the final value of `PTR_DAT_400d069c` is: `0x4a,0xef, 0x4b, 0x8, 0x4c, 0x8, 0x4d, 0x8, 0x4e, 0x8, 0x4f, 0x8, 0x46, 0x43, 0x53, 0x43` and the extracted `PTR_BYTE_400d06a0` is: `0x79, 0x8D, 0x2D, 0x3E, 0x2E, 0x6E, 0x79, 0x6D, 0x28, 0x38, 0x2D, 0x38, 0x77, 0x75, 0x60, 0x21`

now we can get the final flag using this script:
```py
data_400d069c = [0x4a,0xef, 0x4b, 0x8, 0x4c, 0x8, 0x4d, 0x8, 0x4e, 0x8, 0x4f, 0x8, 0x46, 0x43, 0x53, 0x43]
data_400d06a0 = [0x79, 0x8D, 0x2D, 0x3E, 0x2E, 0x6E, 0x79, 0x6D, 0x28, 0x38, 0x2D, 0x38, 0x77, 0x75, 0x60, 0x21]
r = []
for i in range(0x10):
    r.append(data_400d069c[i]^data_400d06a0[i])
print(b'FCSC{'+bytes(r)+b'}')
```


our final flag is : `FCSC{3bf6bf4ef0b0163b}`


