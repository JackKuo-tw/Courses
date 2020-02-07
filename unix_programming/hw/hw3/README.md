## addsub1

```
$ nc rhino.zoolab.org 2500
addsub1:
	eax = 0x8ebee382
	eax = eax + 0xa9e3432f
	eax = eax - 0xdb8f263b
======
======
Enter your codes: (type 'done:' when done)
mov eax, 0x8ebee382
add eax, 0xa9e3432f
sub eax, 0xdb8f263b
done:
======
eax = 0x5d130076 (== 0x5d130076)
*** correct
*** FLAG: ASM{12d30a57b6fd8c9779eb4d64428fb064653a7e00}
======
RAX: 0x5d130076    RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```



## addsub2

```
$ nc rhino.zoolab.org 2501
addsub2:
	final = val1 + val2 - val3
======
      val1 @ 0x600000-600004
      val2 @ 0x600004-600008
      val3 @ 0x600008-60000c
     final @ 0x60000c-600010
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
add eax, [0x600004]
sub eax, [0x600008]
mov [0x60000c], eax
done:
======
final = 0xcf6f336d (== 0xcf6f336d)
*** correct
*** FLAG: ASM{9f4b9cc948960da32302135229f622472f7d8713}
======
RAX: 0xcf6f336d    RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## swapmem

```
$ nc rhino.zoolab.org 2519
swapmem: swap the values in val1 and val2
======
      val1 @ 0x600000-600008
      val2 @ 0x600008-600010
======
Enter your codes: (type 'done:' when done)
mov rax, [0x600000]
mov rbx, [0x600008]
mov [0x600000], rbx
mov [0x600008], rax
done:
======
val1 = 0xbe782cdc (== 0xbe782cdc)
val2 = 0x83d61c9e (== 0x83d61c9e)
*** correct
*** FLAG: ASM{bfc6b67fad445d161d540cfd75a5f57bbabb31ea}
======
RAX: 0x83d61c9e    RBX: 0xbe782cdc    RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## swapreg

用 `xchg` 比較快

```
$ nc rhino.zoolab.org 2520
swapreg: swap the values in RAX and RBX
======
======
Enter your codes: (type 'done:' when done)
xchg rax, rbx
done:
======
rax = 0x93a31b7d (== 0x93a31b7d)
rbx = 0x6ac1d2d5 (== 0x6ac1d2d5)
*** correct
*** FLAG: ASM{534a511af0ecbeb0f7f367a81d416d27749c2642}
======
RAX: 0x93a31b7d    RBX: 0x6ac1d2d5    RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```



```
$ nc rhino.zoolab.org 2520
swapreg: swap the values in RAX and RBX
======
======
Enter your codes: (type 'done:' when done)
xor RAX, RBX
xor RBX, RAX
xor RAX, RBX
done:
======
rax = 0x64a39227 (== 0x64a39227)
rbx = 0x2023f3e0 (== 0x2023f3e0)
*** correct
*** FLAG: ASM{534a511af0ecbeb0f7f367a81d416d27749c2642}
======
RAX: 0x64a39227    RBX: 0x2023f3e0    RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## tolower

```
$ nc rhino.zoolab.org 2521
tolower: convert the single character in val1 to uppercase and store in val2
======
      val1 @ 0x600000-600001
      val2 @ 0x600001-600002
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
xor eax, 0x20
mov [0x600001], eax
done:
======
val1 = 0x73 (== 0x73)
val2 = 0x53 (== 0x53)
*** correct
*** FLAG: ASM{a2ae0dfc6e3d5f1a32bdc8bc9a12382a2c8fbb8b}
======
RAX: 0x53          RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## ul+lu

```
$ nc rhino.zoolab.org 2522
ul+lu: convert the alphabet in CH from upper to lower or from lower to upper
======
======
Enter your codes: (type 'done:' when done)
xor ch, 0x20
done:
======
ch = 0x52 (== 0x52)
*** correct
*** FLAG: ASM{8c9f0650d45e9a8cf1a0c6696f486025e30005ad}
======
RAX: 0x0           RBX: 0x0           RCX: 0x5200        RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## clear17

```
$ nc rhino.zoolab.org 2503
clear17: clear bit-17 in eax (zero-based index)
======
======
Enter your codes: (type 'done:' when done)
xor eax, 0x20000
done:
======
eax = 0x4dadce41 (== 0x4dadce41)
*** correct
*** FLAG: ASM{495071da9e7398c46f7b924f4038a597d3952314}
======
RAX: 0x4dadce41    RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## dec2ascii

```
$ nc rhino.zoolab.org 2504
dec2ascii: convert the value (0-9) in AL to its ASCII character
======
======
Enter your codes: (type 'done:' when done)
add al, '0'
done:
======
al = 0x37 (== 0x37)
*** correct
*** FLAG: ASM{067d91cabbe48ae0d378b9f06e09a3f5dd19f192}
======
RAX: 0x37          RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## dispbin

需注意每個 byte 長度對應的 register 長度，避免使用過長的 reg 導致放入時壓到其他 byte。

```
$ nc rhino.zoolab.org 2505
dispbin:
	given a number in AX, store the corresponding bit string in str1.
	for example, if AX = 0x1234, the result should be:
	str1 = 0001001000111000
======
      str1 @ 0x600000-600014
======
Enter your codes: (type 'done:' when done)
    mov rdx, 0
L1:
    mov bx, ax
    and bx, 0x1
    add bx, '0'
    mov r8, 0x600000
    add r8, 0xf
    sub r8, rdx
    mov [r8], bl
    shr ax, 1
    inc rdx
    cmp rdx, 0x10
    jb L1
done:
======
str1 = b'0111101101011011\x00\x00\x00\x00' (== b'0111101101011011\x00\x00\x00\x00')
*** correct
*** FLAG: ASM{7cd0546f6f0ab256514c5426156e0642a0c5ec08}
======
RAX: 0x0           RBX: 0x30          RCX: 0x0           RDX: 0x10
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## eval1

```
$ nc rhino.zoolab.org 2506
eval1:
	Rval = -Xval + (Yval – Zval)
======
      Xval @ 0x600000-600004
      Yval @ 0x600004-600008
      Zval @ 0x600008-60000c
      Rval @ 0x60000c-600010
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
lea ebx, [0x600004]
lea ecx, [0x600008]
neg eax
add eax, [ebx]
sub eax, [ecx]
mov [0x60000c], eax
done:
======
Rval = 0xe8e805c6 (== 0xe8e805c6)
*** correct
*** FLAG: ASM{e683b6b39c904540bf724e1e8a8e9ef030f01e29}
======
RAX: 0xe8e805c6    RBX: 0x600004      RCX: 0x600008      RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

##  isolatebit

因為 val1 只有 1 byte 空間，因此用 al 去填入才是剛好的。

```
$ nc rhino.zoolab.org 2507
isolatebit:
	get the value bit-11 ~ bit-5 in AX and store the result in val1
	(zero-based bit index)
======
      val1 @ 0x600000-600001
      val2 @ 0x600001-600002
======
Enter your codes: (type 'done:' when done)
sar ax, 5
and ax, 0xff
mov [0x600000], al
done:
======
val1 = 0x6a (== 0x6a)
val2 = 0x63 (== 0x63)
*** correct
*** FLAG: ASM{c68c96817b844512055b5aa636fc71eba803a77c}
======
RAX: 0x6a          RBX: 0xad57        RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## leax

要注意 lea 後面中括號內，乘數必須是 1, 2, 4, 8

```
leax:
	eax = edi * 2
	ebx = edi * 3
	ecx = edi * 5
	edx = edi * 9
======
======
Enter your codes: (type 'done:' when done)
lea eax, [edi*2]
lea ebx, [edi*2+edi]
lea ecx, [edi*4+edi]
lea edx, [edi*8+edi]
done:
======
eax = 0xc9ca1080 (== 0xc9ca1080)
ebx = 0x2eaf18c0 (== 0x2eaf18c0)
ecx = 0xf8792940 (== 0xf8792940)
edx = 0x8c0d4a40 (== 0x8c0d4a40)
*** correct
*** FLAG: ASM{4dcff917b5e26def74682294894946603a1e6b6b}
======
RAX: 0xc9ca1080    RBX: 0x2eaf18c0    RCX: 0xf8792940    RDX: 0x8c0d4a40
RDI: 0x64e50840    RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## loop

因為要變小，因此 or 0x20 即可

```
loop15:
	str1 is a string contains 15 lowercase and uppercase alphbets.
	implement a loop to convert all alplabets to lowercase,
	and store the result in str2.
======
      str1 @ 0x600000-600010
      str2 @ 0x600010-600020
======
Enter your codes: (type 'done:' when done)
    mov rcx, 0
L1:
    mov r8, 0x600000
    add r8, rcx
    mov eax, [r8]
    or eax, 0x20
    mov r8, 0x600010
    add r8, rcx
    mov [r8], eax
    inc rcx
    cmp rcx, 0xf
    jb L1
done:
======
str2 = b'zmobahsndpywvsu\x00' (== b'zmobahsndpywvsu\x00')
*** correct
*** FLAG: ASM{44f8c5d95bfc5a2cebf28e8a0016bf0c898e42b7}
======
RAX: 0x6d7a0075    RBX: 0x0           RCX: 0xf           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## math1

```
math1: unsigned arithmetic
	var4 = (var1 + var2) * var3
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
add eax, [0x600004]
mov ebx, [0x600008]
mul ebx
mov [0x60000c], eax
done:
======
var1 = 0x131b8d71 (== 0x131b8d71)
var2 = 0x4a5e8563 (== 0x4a5e8563)
var3 = 0x5b95c4d4 (== 0x5b95c4d4)
var4 = 0x30e5e790 (== 0x30e5e790)
*** correct
*** FLAG: ASM{329d6b2d2b60dbe275c2fadfab47326ec7ce8032}
======
RAX: 0x30e5e790    RBX: 0x5b95c4d4    RCX: 0x0           RDX: 0x2171149d
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## math2

有號的乘法

```
math2: signed arithmetic
	eax = (-var1 * var2) + var3
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
neg eax
mov ebx, [0x600004]
imul ebx
mov ecx, [0x600008]
add eax, ecx
done:
======
var1 = 0x16daeab4 (== 0x16daeab4)
var2 = 0x41dfb00c (== 0x41dfb00c)
var3 = 0x6106c133 (== 0x6106c133)
eax = 0xa69c00c3 (== 0xa69c00c3)
*** correct
*** FLAG: ASM{a12829814c0c3681044eaf4fee319c66994e2fda}
======
RAX: 0xa69c00c3    RBX: 0x41dfb00c    RCX: 0x6106c133    RDX: 0xfa1e71fe
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## math3

這題要注意需把 overflow 部分清掉

```
math3: 32-bit unsigned arithmetic
	var4 = (var1 * 5) / (var2 - 3)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var4 @ 0x600008-60000c
======
Enter your codes: (type 'done:' when done)

mov eax, [0x600000]
mov ecx, 5
mul ecx
mov ebx, [0x600004]
sub ebx, 3
mov edx, 0
div ebx
mov [0x600008], eax
done:
======
var1 = 0x5f8c4bf9 (== 0x5f8c4bf9)
var2 = 0x1206f796 (== 0x1206f796)
var4 = 0xc (== 0xc)
*** correct
*** FLAG: ASM{29fefc836582fc2520b7c1943a4f222d13c867bd}
======
RAX: 0xc           RBX: 0x1206f793    RCX: 0x5           RDX: 0x569e0f9
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## math4

要加上 [cdq](https://www.aldeid.com/wiki/X86-assembly/Instructions/cdq)，通常在做有號除法前都會做這件事情

```
math4: 32-bit signed arithmetic
	var4 = (var1 * -5) / (-var2 % var3)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600004]
neg eax
mov ebx, [0x600008]
cdq
idiv ebx
mov ecx, edx ; save remainder into ecx
mov eax, [0x600000]
mov r9, -5
imul r9
mov edx, 0
cdq
idiv ecx
mov [0x60000c], eax
done:
======
var1 = 0x56260e92 (== 0x56260e92)
var2 = 0x64d93884 (== 0x64d93884)
var3 = 0x67c3a69c (== 0x67c3a69c)
var4 = 0x0 (== 0x0)
*** correct
*** FLAG: ASM{5df848046cc0a617a07df92cc063abe6eb9fab33}
======
RAX: 0x0           RBX: 0x67c3a69c    RCX: 0x9b26c77c    RDX: 0x5141b726
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## math5

```
$ nc rhino.zoolab.org 2514
math5: 32-bit signed arithmetic
	var3 = (var1 * -var2) / (var3 - ebx)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
mov ecx, [0x600004]
neg ecx
imul ecx
mov edx, 0
mov ecx, [0x600008]
sub ecx, ebx
cdq
idiv ecx
mov [0x600008], eax
done:
======
var1 = 0x2502264d (== 0x2502264d)
var2 = 0x43f24d46 (== 0x43f24d46)
var3 = 0x50 (== 0x50)
*** correct
*** FLAG: ASM{d094846539de42b83a8c47494e8ad906f4642340}
======
RAX: 0x50          RBX: 0x3d0d0e60    RCX: 0xff99f43c    RDX: 0xfffe0b32
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## mulbyshift

```
mulbyshift: multiply val1 by 26 and store the result in val2
======
      val1 @ 0x600000-600004
      val2 @ 0x600004-600008
======
Enter your codes: (type 'done:' when done)
mov eax, [0x600000]
mov ebx, eax
mov ecx, eax
sal eax, 4
sal ebx, 3
sal ecx, 1
add eax, ebx
add eax, ecx
mov [0x600004], eax
done:
======
val1 = 0xa441 (== 0xa441)
val2 = 0x10ae9a (== 0x10ae9a)
*** correct
*** FLAG: ASM{34b9caa05654006ec6a14db1a60ea145b2689b20}
======
RAX: 0x10ae9a      RBX: 0x52208       RCX: 0x14882       RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```



## posneg

```
$ nc rhino.zoolab.org 2517
posneg: test if registers are positive or negative.
	if ( eax >= 0 ) { var1 = 1 } else { var1 = -1 }
	if ( ebx >= 0 ) { var2 = 1 } else { var2 = -1 }
	if ( ecx >= 0 ) { var3 = 1 } else { var3 = -1 }
	if ( edx >= 0 ) { var4 = 1 } else { var4 = -1 }
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
Enter your codes: (type 'done:' when done)
    mov edi, 0x600000
    mov r8, -1
    mov r9, 1
    test eax, eax
    jge SGA
    mov [edi], r8
    jmp TB
SGA:
    mov [edi], r9
TB:
    test ebx, ebx
    jge SGB
    mov [edi+4], r8
    jmp TC
SGB:
    mov [edi+4], r9
TC:
    test ecx, ecx
    jge SGC
    mov [edi+8], r8
    jmp TD
SGC:
    mov [edi+8], r9
TD:
    test edx, edx
    jge SGD
    mov [edi+0xc], r8
SGD:
    mov [edi+0xc], r9
done:
======
var1 = 0x1 (== 0x1)
var2 = 0xffffffff (== 0xffffffff)
var3 = 0x1 (== 0x1)
var4 = 0x1 (== 0x1)
*** correct
*** FLAG: ASM{4a637ac9bd775a6f7ac8c15290a7bae3c5f492e1}
======
RAX: 0x46e2d945    RBX: 0x912245a3    RCX: 0x1d710232    RDX: 0x67374e8d
RDI: 0x600000      RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## minicall

這題為何 ret 在 rax 勒？

因為在 call function 時會把 return address 放在 stack 上面，我們把它拿出來就好。

但是拿出來就沒了，ret 回不去，因此要再去 push rax。

```
$ nc rhino.zoolab.org 2515
minicall: implement a minimal function call in the emulator
===== THE CODE
    call   a
    jmp    exit

a:  ; function a - read ret-addr in rax
    pop    rax
    push   rax
    ret
exit:
======
======
Enter your codes: (type 'done:' when done)
    call a
    jmp  exit
a:
    pop rax
    push rax
    ret
exit:
done:
======
eax = 0x400005 (== 0x400005)
*** correct
*** FLAG: ASM{6a409552c7451c41c5e961cc5a9af9ced510ce98}
======
RAX: 0x400005      RBX: 0x0           RCX: 0x0           RDX: 0x0
RDI: 0x0           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

## recur

```
recur: implement a recursive function

   r(n) = 0, if n <= 0
        = 1, if n == 1
        = 2*r(n-1) + 3*r(n-2), otherwise

   please call r(24) and store the result in RAX
======
======
Enter your codes: (type 'done:' when done)
    mov rdi, 24 ; total
    mov rcx, 0  ; counter
    call r
    jmp exit
r:
    cmp rcx, 0
    jg  nzero
    mov rax, 0
    jmp loop
nzero:
    cmp rcx, 1
    jg none
    mov rbx, rax ; r(n-1) in rbx
    mov rax, 1
    jmp loop
none:
    mov rsi, rbx ; r(n-2) in rsi
    mov rbx, rax
    mov r8, 2
    mul r8
    mov r8, 3
    mov r9, rax
    mov rax, rsi
    mul r8
    add rax, r9
loop:
    inc rcx
    cmp rcx, rdi
    jle r
exit:
done:
======
rax = 0x1070872e38 (== 0x1070872e38)
*** correct
*** FLAG: ASM{432ea21a5fc13ac8e625fa2d22ae02745289a4d8}
======
RAX: 0x1070872e38  RBX: 0x57ad7ba13   RCX: 0x19          RDX: 0x0
RDI: 0x18          RSI: 0x1d39d3e06   RBP: 0x0           RSP: 0x9ffff8
```

## bubble

```
bubble: bubble sort for 10 integers
======
      a[0] @ 0x600000-600004
      a[1] @ 0x600004-600008
      a[2] @ 0x600008-60000c
      a[3] @ 0x60000c-600010
      a[4] @ 0x600010-600014
      a[5] @ 0x600014-600018
      a[6] @ 0x600018-60001c
      a[7] @ 0x60001c-600020
      a[8] @ 0x600020-600024
      a[9] @ 0x600024-600028
======
Enter your codes: (type 'done:' when done)
_start:
    mov rdi, 0 ; i
    mov rsi, 0 ; j
loopi:
loopj:
    mov eax, [0x600000 + rsi*4]
    mov ebx, [0x600000 + rsi*4 + 4]
    cmp eax, ebx
    jl incj
swap:
    mov [0x600000 + rsi*4], ebx
    mov [0x600000 + rsi*4 + 4], eax
incj:
    inc rsi
    cmp rsi, 8
    jl loopj
inci:
    mov rsi, 0
    inc rdi
    cmp rdi, 8
    jl loopi
done:
======
a[0] = 0xef (== 0xef)
a[1] = 0x16e (== 0x16e)
a[2] = 0x18c (== 0x18c)
a[3] = 0x1f6 (== 0x1f6)
a[4] = 0x21c (== 0x21c)
a[5] = 0x22a (== 0x22a)
a[6] = 0x256 (== 0x256)
a[7] = 0x2ef (== 0x2ef)
a[8] = 0x35b (== 0x35b)
a[9] = 0x399 (== 0x399)
*** correct
*** FLAG: ASM{ed11e3273e0a8845d42afa3f901074df13e12d0e}
======
RAX: 0x2ef         RBX: 0x35b         RCX: 0x0           RDX: 0x0
RDI: 0x8           RSI: 0x0           RBP: 0x0           RSP: 0xa00000
```

