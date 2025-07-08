---
author: "nothoudaifa"
title: "writeup for rev/r3loads chall from r3ctf 2025"
date: "2025-07-08"
tags: 
- ctf 
- rev
- autorev
---


```
points: 929 pts
solves: 11 solves
```
this weekend i played r3ctf, i solved this challenge with @aymenmog, i really enjoyed doing this autorev, so i decided to make this writeup.
## Initial analysis

this is an autorev chall, we are given 11423 executables each one is in it’s directory along with a set of shared libraries, the readme says that each binary takes 8 bytes input and final output of the chall will be a jpg which is the concatenation of 11423 right inputs.

each part is like this:

```bash
player@notarch:~/ctfs/r3/r3loads/game/300$ ls
beatme                               RRRRRRRRRRRR3R33RRRR3R3R3R3RRR33.so  RRRRRRRRRRRR3R33RRRRRRRRRRR33333.so  RRRRRRRRRRRR3R3R33333RR3R33333RR.so
RRRRRRRRRRRR3R33RRR3R3RRRR333RRR.so  RRRRRRRRRRRR3R33RRRR3R3R3R3RRRR3.so  RRRRRRRRRRRR3R33RRRRRRRRRRR333R3.so  RRRRRRRRRRRR3R3R33333RR3R3333R3R.so
RRRRRRRRRRRR3R33RRR3R3RRRR33R33R.so  RRRRRRRRRRRR3R33RRRRR33R3R33333R.so  RRRRRRRRRRRR3R33RRRRRRRRRRR33R33.so  RRRRRRRRRRRR3R3R33333RR3R3333RRR.so
RRRRRRRRRRRR3R33RRR3R3RRRR33R3RR.so  RRRRRRRRRRRR3R33RRRRR33R3R3333RR.so  RRRRRRRRRRRR3R33RRRRRRRRRRR33RR3.so  RRRRRRRRRRRR3R3R33333RR3R333R33R.so
RRRRRRRRRRRR3R33RRRR33333R33RRR3.so  RRRRRRRRRRRR3R33RRRRR33R3R333R3R.so  RRRRRRRRRRRR3R33RRRRRRRRRRR3R333.so  RRRRRRRRRRRR3R3R3333R3R33RRR333R.so
RRRRRRRRRRRR3R33RRRR33333R3R3333.so  RRRRRRRRRRRR3R33RRRRR33R3R333RRR.so  RRRRRRRRRRRR3R33RRRRRRRRRRR3R3R3.so  RRRRRRRRRRRR3R3R3333R3R33RRR33RR.so
RRRRRRRRRRRR3R33RRRR33333R3R33R3.so  RRRRRRRRRRRR3R33RRRRR33R3R33R33R.so  RRRRRRRRRRRR3R3R3333333RRR33RRR3.so  RRRRRRRRRRRR3R3R3333R3R33RRR3R3R.so
RRRRRRRRRRRR3R33RRRR33333R3R3R33.so  RRRRRRRRRRRR3R33RRRRR33R3R33R3RR.so  RRRRRRRRRRRR3R3R3333333RRR3R3333.so  RRRRRRRRRRRR3R3R3333R3R33RRR3RRR.so
RRRRRRRRRRRR3R33RRRR33333R3R3RR3.so  RRRRRRRRRRRR3R33RRRRRR333333R333.so  RRRRRRRRRRRR3R3R3333333RRR3R33R3.so  RRRRRRRRRRRR3R3R3333R3R33RRRR33R.so
RRRRRRRRRRRR3R33RRRR3R3R3R3RR3R3.so  RRRRRRRRRRRR3R33RRRRRR333333R3R3.so  RRRRRRRRRRRR3R3R3333333RRR3R3R33.so  RRRRRRRRRRRR3R3R3333RR3RR33RRRRR.so 
```

running it gives us:

```bash
player@notarch:~/ctfs/r3/r3loads/game/300$ ./beatme
Weclome to r3ctf 2025, can you beat me? 300/11423
Input something: 
aaaabbbb
Try again!
```

we will first starting by reversing a binary, then see what we can automate and script it.

## the “rev” part:

loading one executable in ida we get [this](/r3ctf-r3loads/main-decomp.c) (variable declarations removed for clarity), this main function can be separated into 3 parts:

#### Part 1 of main: loading the functions

the first thing it does is load 10 functions (which will be used later)  from the dynamic libraries in the same directory, it uses dlopen to load the library and dlsym to get the function from it, i will show how i got what one function does:

```c
handlee = dlopen("./RRRRRRRRRRRRRRRRRRR33RRR33R3RRRR.so", 1);
  if ( !handlee )
    exit(1);
  dlerror();
  qword_7048 = dlsym(handlee, "RRRRRRRRRRRRRRRRRRR33RRR33R3RRR3");
```

opening this function in binja we get the following:

![Screenshot from 2025-07-07 18-50-52.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_18-50-52.png)

it loads another function from another library and calls it, while only operating on arg3.

it seems that it keeps doing this a few times until it reaches the last function which defines the operation.

the path (in this case it only calls dlsym one time):

![Screenshot from 2025-07-07 18-52-16.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_18-52-16.png)

we can see in the end it returns arg1 - arg2, and also it does not use the third argument, this actually is the case for all the other functions, arg3 is never used, it is likely used as an obfuscation mechanism so we can ignore it.

the functions in order are (note that all arguments and returns are unsigned ints):

```c
xor_first_two => arg1 ^ arg2
read_integer_at_offset => ((unsigned int*)arg1)[arg2]
add_at_arg1_arg2 => *((unsigned int*)arg1) += arg2
and_arg1_arg2 => arg1 & arg2
add_arg1_arg2 => arg1 + arg2
subtract_arg1_arg2 => arg1 - arg2
mult_arg1_arg2 => arg1 * arg2
or_arg1_arg2 => arg1 | arg2
integer_div_arg1_arg2 => arg1 / arg2
bitwise_not => ~arg1
```

after reversing all functions and renaming them we move to the second part.

#### Part 2 of main: Setting the key 

it’s code after renaming the functions is accessible here, the first thing i noticed about this part is that it is all just operations on constants, it seems to be writing four unsigned ints in v91, we can extract it using gdb by setting a breakpoint just before the call to sub_1630, then inspecting the memory at rsi.

#### Part 3 of main: Handling input

```c
puts("Weclome to r3ctf 2025, can you beat me? 1/11423");
  puts("Input something: ");
  buf = 0LL;
  read(0, &buf, 8uLL);
  sub_1630((unsigned int *)&buf, v91);
  if ( buf == 0xF06203EC3D2C5B74LL )
    puts("You win!");
  else
    puts("Try again!");
```

this part read 8 bytes, then calls sub_1630 on it with v91 which is constant and likely used as a key to some encryption on buf.

### reversing the encryption function:

the encrypt function decompilation is [here](/r3ctf-r3loads/encrypt-decomp.c), it seems very scary, but we will go at it bit by bit till we understand it.

at first, it takes our 8 bytes input and splits it into two unsigned ints, `first` and `second`, then it sets `S` a State array (similar to rc4) using the key (this array is only read from), then it loops 0x478 times while doing some operations on `first` and `second`, in the end it writes them back to our input. 

now before i started reversing this function, i noticed that there are four regions of code that only use constant values, i marked them on the code above, here is an example one:

```c
v162 = mult_arg1_arg2;                      
    v159 = and_arg1_arg2;
    v154 = add_arg1_arg2(3879220815LL, 1303544047LL, 1673289997LL);
    v148 = or_arg1_arg2(119385303LL, 3231329148LL, 141954786LL);
    v145 = bitwise_not;
    v137 = and_arg1_arg2(1503872738LL, 978965565LL, 1470160614LL);
    v125 = mult_arg1_arg2(761654413LL, 3043887596LL, 4085454309LL);
    v120 = mult_arg1_arg2;
    v131 = xor_first_two(3333670936LL, 2147831941LL, 2217132274LL);
    v115 = add_arg1_arg2(4141055499LL, 1038626365LL, 1517838417LL);
    v112 = xor_first_two;
    v109 = add_arg1_arg2(2686461105LL, 3867813406LL, 686930351LL);
    v106 = xor_first_two(3259940977LL, 1820144441LL, 2738368048LL);
    v103 = mult_arg1_arg2;
    v2 = bitwise_not(2920661428LL, 408102610LL, 758347707LL);
    v3 = xor_first_two(2688446990LL, 3924691225LL, 3780184521LL);
    v4 = or_arg1_arg2;
    v5 = xor_first_two(715168552LL, 4203380807LL, 4042894304LL);
    v6 = and_arg1_arg2(3362982059LL, 1951226027LL, 2091265364LL);
    v7 = mult_arg1_arg2(2447727191LL, 4231433857LL, 2356928693LL);
    v8 = v4(v7, v6, v5);
    v9 = v103(v8, v3, v2);
    v10 = v112(v9, v106, v109);
    v11 = v120(v10, v115, v131);
    v12 = v145(v11, v125, v137);
    LODWORD(v159) = v159(v12, v148, v154);
    v155 = mult_arg1_arg2;
    v149 = integer_div_arg1_arg2;
    v146 = and_arg1_arg2;
    v138 = add_arg1_arg2(3879220815LL, 1303544047LL, 1673289997LL);
    v126 = or_arg1_arg2(119385303LL, 3231329148LL, 141954786LL);
    v121 = bitwise_not;
    v132 = and_arg1_arg2(1503872738LL, 978965565LL, 1470160614LL);
    v116 = mult_arg1_arg2(761654413LL, 3043887596LL, 4085454309LL);
    v113 = mult_arg1_arg2;
    v110 = xor_first_two(3333670936LL, 2147831941LL, 2217132274LL);
    v107 = add_arg1_arg2(4141055499LL, 1038626365LL, 1517838417LL);
    v104 = xor_first_two;
    v102 = add_arg1_arg2(2686461105LL, 3867813406LL, 686930351LL);
    v101 = xor_first_two(3259940977LL, 1820144441LL, 2738368048LL);
    v100 = mult_arg1_arg2;
    v13 = bitwise_not(2920661428LL, 408102610LL, 758347707LL);
    v14 = xor_first_two(2688446990LL, 3924691225LL, 3780184521LL);
    v15 = or_arg1_arg2;
    v16 = xor_first_two(715168552LL, 4203380807LL, 4042894304LL);
    v17 = and_arg1_arg2(3362982059LL, 1951226027LL, 2091265364LL);
    v18 = mult_arg1_arg2(2447727191LL, 4231433857LL, 2356928693LL);
    v19 = v15(v18, v17, v16);
    v20 = v100(v19, v14, v13);
    v21 = v104(v20, v101, v102);
    v22 = v113(v21, v107, v110);
    v23 = v121(v22, v116, v132);
    v24 = v146(v23, v126, v138);
    v25 = v149(v24, 7LL, 1501804614LL);
    v26 = v155(v25, 7LL, 3199106012LL);
    v27 = pow(2LL, (unsigned int)((_DWORD)v159 - v26 + 1), 3474473344LL);
```

it basically does some operations then calls `pow(2, result)` (third arg of pow is not used), then the rest of the logic uses it’s result for calculations, this pattern repeats four times inside the loop, we can replace those regions by their final results in the decompilation to simplify it.

to get this four constants i used gdb with dprintf (setting the breakpoint at the final instruction before pow returns) to print the return value of pow.

```c
dprintf *0x162E+0x555555554000, "rax = 0x%02x\n", $rax
```

we get these values

```c
rax = 0x08
rax = 0x08
rax = 0x20
rax = 0x10
rax = 0x08
rax = 0x08
rax = 0x20
rax = 0x10
```

and sure enough, these four values kept repeating, meaning my observation was right, now putting them inside the loop gives us the following:

```c
do
  {
    v172 = v168 & 3LL;
    v27 = 0x8; // static 1 output
    v173 = second * v27
    v47 = 0x8; // static 2 output
    v174 = second / v47;
    v175 = S[v172];
    v175 = v175+v168;
    v176 = v173^v174;
    v176 = v176+second;
    v51 = ~v175;
    v29 = v51 & v176;
    v53 = ~v176;
    v54 = v53 & v175;
    v55 = v54 | v29;
    first += v55;
    
    v72 =0x20; // static 3 output
    v177 = first / v72;
    v88 = 0x10; // static 4 output
    v178 = first * v88;
    v91 = ~v177;
    v80 = v91 & v178;
    v93 = ~v178;
    v94 = v93 & v177;
    v179 = v94 | v80;
    v168 +=  3284565212;
    v180 = v179 + first;
    v96 = v168 / 0x800LL;
    v181 = 255LL & v96;
    v182 = S[v181]
    v183 = v182 + v168;
    v98 = v180 ^ v183;
    second += v98;
    v185 += 1uLL;
  }
  while ( 0x478 >= v185 );
```

now as you can see, this is pretty good c decompilation, i can reverse it but i still used llm to turn it into a more simplified c version, so i can use it for scripting, the four static constants are used for multiplication and division, and since they are powers of 2, they are simplified to right and left shifts. it is also using `v168` to index into the state array, which is incremented by a constant value 0x9D175C01 (note that the addition overflows, and the result will be cut to fit 32bits).



```c
uint64_t enc(uint32_t *input, uint32_t *key) {
    uint32_t S[256];
    uint32_t first = input[0];
    uint32_t second = input[1];
    uint32_t v168 = 0;
    uint64_t v185 = 0;

    // Initialize S-box
    for (int i = 0; i <= 255; ++i)
        S[i] = i;

    // RC4-style key scheduling
    uint32_t v170 = 0, v171 = 0;
    for (int i = 0; i <= 255; ++i) {
        uint32_t v184 = S[i];
        v170 = (v184 + v170 + key[v171]) & 0xFF;
        uint32_t tmp = S[i];
        S[i] = S[v170];
        S[v170] = tmp;

        if (++v171 > 3)
            v171 = 0;
    }

    do {
        uint32_t v172 = v168 & 3; // 7. we can get this after decrementig v168 by the constant
        uint32_t v175 = (S[v172] + v168); // 8. obviously we got this
        uint32_t v176 = ((second << 3) ^ (second >> 3)) + second; // 9. this is using the second before encrypting it, which we decrypted in step 6
        first += v175 ^ v176;  // 10. this we can get since.

        // Second part operations
        uint32_t v179 = (first >> 5) ^ (first << 4); // 5. v179 we have since it's using the first after it is incremented (it's using the encrypted first)
        v168 += 0x9D175C01U;  // Constant increment
        uint32_t v180 = v179 + first; // 6. since we have this we can get the decrypted value of second (by subtracting the xor result)

        // Final operations
        uint32_t v181 = (v168 >> 11) & 0xFF; // 2. we have this value since we have v168
        uint32_t v183 = S[v181] + v168; // 3. this too since we just calculated v181
        second += v180 ^ v183; // 4. to get this we need v180

        v185 += 1;
    } while (v185 <= 0x478);

    // 1. we start here, where we have first and second and the last value of v168

    input[0] = first;
    input[1] = second;

}
```

now our task is to reverse this function, i have written my line of thought for reversing it's operations as comments in the code above, follow steps 1 to 10.

here is the final decryption script:
```c
uint64_t dec(uint32_t first, uint32_t second, uint32_t *key, uint32_t* output) {
    uint32_t S[256];
    uint32_t v168 = 0;
    uint64_t v185 = 0;

    // Initialize S-box
    for (int i = 0; i <= 255; ++i)
        S[i] = i;

    // RC4-style key scheduling
    uint32_t v170 = 0, v171 = 0;
    for (int i = 0; i <= 255; ++i) {
        uint32_t v184 = S[i];
        v170 = (v184 + v170 + key[v171]) & 0xFF;
        uint32_t tmp = S[i];
        S[i] = S[v170];
        S[v170] = tmp;

        if (++v171 > 3)
            v171 = 0;
    }

    v168 = 0xa2c473fc; // this is the last value of v168

    do {
        uint32_t v181 = (v168 >> 11) & 0xFF;
        uint32_t v183 = S[v181] + v168;

        uint32_t v179 = (first >> 5) ^ (first << 4);

        uint32_t v180 = v179 + first;

        second -= v180 ^ v183;

        v168-=0x9D175C01; // we are going backwards now

        uint32_t v172 = v168 & 3; // this we have, just decrement v168 
        uint32_t v175 = (S[v172] + v168); // this too we have
                                          
        uint32_t v176 = ((second << 3) ^ (second >> 3)) + second; // use the decrypted second value to get this value
                                          
        first -= v175 ^ v176;  // xor this to get the decrypted  first value
        v185+=1;

    }
    while (v185 <= 0x478);
    

    printf("first = 0x%x , second = 0x%x\n", first, second);

    output[0] = first;
    output[1]=second;
    

}
```

using it on the cipher 0xF06203EC3D2C5B74 for the level1 with the key extracted using gdb we get

![Screenshot from 2025-07-07 20-13-17.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_20-13-17.png)

it worked!!

## the “auto” part:

to start scripting decryption for the 11423 executables we first need to know what changes between them, opening the second executable and reversing it (just like in the “rev” part) i noticed that four things change:

- the key (v91)
- the constant that increments v168 is different
- the four static values that are calculated inside the loop and used for shifting
- and obviously the cipher text

to get the right input for any executable we have to extract these values, but first i updated my decrypt function so it uses dynamic keys, constant and shifts, you can check it [here](/r3ctf-r3loads/dynamic-decrypt.c).

to extract the values dynamicly i used libdebug, since it is waaaaay faster than gdb scripting (gdb scripting would have took hours).

#### Extracting the key

we start by extracting the key, to do that i set a breakpoint at read then return from read and go four instructions after it, to reach the call for the encrypt function

![Screenshot from 2025-07-07 21-16-33.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_21-16-33.png)

after that i read `4*sizeof(unsigned int)` bytes of memory starting at rsi to get the key.

```python
from libdebug import debugger
d = debugger("./beatme")

pipe= d.run()
pipe.send(b'a'*8) # send dummy data so it does not get stuck at read
d.breakpoint('read')

d.cont() # now we are the start of read
d.finish() # return for the read function
for _ in range(4):
		d.step()
# now we are at "call encrypt"
rsi  = d.regs.rsi
data = d.memory[rsi:rsi+0x10]
key  = [int.from_bytes(data[i:i+4], 'little') for i in range(0, 0x10, 4)] 
```

#### Extracting the cipher
to extract the cipher from the binary, i noticed that the instructions that do the loading and  comparision are always the same

![Screenshot from 2025-07-07 21-37-48.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_21-37-48.png)

bytes 8b45a83d are the mov instruction and the first byte of cmp instruction, i use these to find their offset in the binary then add 4 and read a dword.

```python
with open(f"./beatme", 'rb') as f:
        filedata = f.read()
a = filedata.find(b'\x8b\x45\xa8\x3d')+4 # mov    eax,DWORD PTR [rbp-0x58] ; cmp
b = filedata.find(b'\x8b\x45\xac\x3d')+4 # mov    eax,DWORD PTR [rbp-0x54] ; cmp

cipher = int.from_bytes(filedata[a:a+4], 'little') | (int.from_bytes(filedata[b:b+4], 'little') << 32)
```

#### Extracting the shift values

now we need to extract the shift values, to do that i decided to set a breakpoint at the end of the pow function and print the value at rax (return value).

![Screenshot from 2025-07-07 22-04-49.png](/r3ctf-r3loads/Screenshot_from_2025-07-07_22-04-49.png)

breakpoint is set at 0x162f.

i knew by observation the the pow function was the same in all executables, and since we can’t hard code the address of pow cause it changes, i decided to search for the offset of pow by it’s first bytes, and we add the size of it, to reach it’s last instruction.

```python
powstart = b'\xf3\x0f\x1e\xfaUH\x89\xe5\x89}\xec\x89u\xe8\x89U\xe4\x83}\xe8\x00'
addr = filedata.find(powstart)+0x3e # addr points to ret instruction of pow
```

now we use this to set a breakpoint and print the values at eax

```python
# after the code that extracts the key
d.breakpoint(addr, file='binary')

shifts = []
for _ in range(4):
		d.cont()
    shifts.append(math.log2(d.regs.rax))
    # we use math.log2 since divising by 0x8
    # is the same as shifting to right by 3 (log2(8) == 3)
```

#### Extracting the constant that increments v168

now to get the v168 increment constant c, we know from ida that v168 is stored at rbp-0x494, now each loop calls pow 4 times, so in the fifth call of pow (which we are setting a breapoint at), we would be in the second loop, v168 would be incremented one time, it would be equal to the increment constant, so we just get the value at v168.

```python
# after the shifts extraction
d.cont()
rbp = d.regs.rbp
c = int.from_bytes(d.memory[rbp-0x494:rbp-0x494+4], 'little')
```

and that’s it!

after this we store all the extracted values in files. and we use them in the decryption in c, i did this because my decryption was in c.

here are the final [extract.py](/r3ctf-r3loads/extract.py) and [decrypt.c](/r3ctf-r3loads/decrypt.c) scripts.

after running them we get this jpg

![output.jpg](/r3ctf-r3loads/output.jpg)
