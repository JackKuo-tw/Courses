#!/usr/bin/env python3
from pwn import *
import binascii

"""
這題使用 python2 有一定機率的編碼會錯誤，因此改用 python3
但還是有一定機率會錯，就多做幾次吧～
"""

r = remote('rayfish.zoolab.org', 2530);
context.arch = "amd64"


def asm2hex(asm_str):
    asm_list = asm_str.split("\n")
    ans = ""
    for line in asm_list:
        instruction = line[32:].split()
        ans += " ".join(instruction) + '\n'
    ans = ans.replace(",", ", ")
    return ans


for i in range(7):
    print(r.recvline().decode('utf8'))

for i in range(19):
    q = r.recvline().split()[1].strip()
    print("question: " + q.decode("utf8"))
    #a = disasm(q.decode('hex')) # python2
    #r.sendlineafter(": ", asm2hex(a).encode("hex")) # python2
    a = disasm(binascii.a2b_hex(q))
    print("ans: " + binascii.hexlify(asm2hex(a).encode()).decode("utf8"))
    r.sendlineafter(": ", binascii.hexlify(asm2hex(a).encode()))
    print(r.recvline().decode("utf8"))
    print(r.recvline())
q = r.recvline().split()[1].strip()
print("question: " + q)
#a = disasm(q.decode('hex')) # python2
#r.sendlineafter(": ", asm2hex(a).encode("hex")) # python2
a = disasm(binascii.a2b_hex(q))
r.sendlineafter(": ", binascii.hexlify(asm2hex(a).encode()))
r.interactive()

