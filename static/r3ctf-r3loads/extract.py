from libdebug import debugger,libcontext
from pwn import p32,p64
import os
import math

libcontext.sym_lvl = 3
keysb = b''
shiftsb = b''
cb = b''
cipherb = b''
for n in range(1, 11423+1):
    print(n)
    os.chdir(f'./{n}')

    with open(f"./beatme", 'rb') as f:
        filedata = f.read()

    a = filedata.find(b'\x8b\x45\xa8\x3d')+4
    b = filedata.find(b'\x8b\x45\xac\x3d')+4

    cipher = int.from_bytes(filedata[a:a+4], 'little') | (int.from_bytes(filedata[b:b+4], 'little') << 32)
    
    powstart = b'\xf3\x0f\x1e\xfaUH\x89\xe5\x89}\xec\x89u\xe8\x89U\xe4\x83}\xe8\x00'
    addr = filedata.find(powstart)+0x3e

    d = debugger("./beatme")


    pipe= d.run()
    pipe.send(b'aaaabbbb\n')
    d.breakpoint('read')
    d.breakpoint(addr, file='binary')

    d.cont()
    d.finish()
    for _ in range(4):
        d.step()
    rsi = d.regs.rsi
    data=  d.memory[rsi:rsi+0x10]
    key = [int.from_bytes(data[i:i+4], 'little') for i in range(0, 0x10, 4)]
    shifts = []
    for _ in range(4):
        d.cont()
        shifts.append(math.log2(d.regs.rax))

    d.cont()
    rbp = d.regs.rbp
    c = int.from_bytes(d.memory[rbp-0x494:rbp-0x494+4], 'little')
    d.step()
    for i in key:
        keysb+= p32(i)

    for i in shifts:
        shiftsb+=p32(int(i))

    cb += p32(c)
    cipherb +=p64(cipher)

    d.kill()
    os.chdir('..')
with open('./keys', 'ab') as f:
    f.write(keysb)
with open('./shifts', 'ab') as f:
    f.write(shiftsb)
with open('./c', 'ab') as f:
    f.write(cb)
with open('./cipher', 'ab') as f:
    f.write(cipherb)
