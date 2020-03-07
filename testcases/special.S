# input a, output factorial(a)
.text
addi $v0, $zero, 11
lb   $a0, 44($fp)
syscall
lb   $a0, 45($fp)
syscall
lb   $a0, 46($fp)
syscall
lb   $a0, 47($fp)
syscall
lb   $a0, 48($fp)
syscall
lb   $a0, 0($fp)
syscall

addi $v0, $zero, 4
addi $a0, $fp, 4
syscall
addi $a0, $fp, 0
syscall

addi $a0, $fp, 12
syscall
addi $a0, $fp, 0
syscall

addi $a0, $fp, 24
syscall
addi $a0, $fp, 0
syscall

addi $v0, $zero, 1
lw $a0, 32($fp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($fp)
syscall
addi $v0, $zero, 1
lw $a0, 36($fp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($fp)
syscall
addi $v0, $zero, 1
lwr $a0, 40($fp)
syscall
addi $v0, $zero, 10
syscall

.data
endl: .asciiz "\n" # 0
fxxk1: .asciiz "\x7B\x7b\175\175\?\"" # 4   #{{}}?"
fxxk2: .asciiz "\\123\\xaf3" #12    # \123\xaf3
fxxk3: .asciiz "0 1\t2\'"  #24       # 0<blank>1<tab>2'
w: .word 0x123fabcd #32 306162637
k0: .word 114514     #36 114514
1k: .word -114514     # 40 114514
a: .byte 'a' # 44
b: .byte '\x62' # 45
c: .byte '\143' # 46
d: .byte '\''  # 47
e: .byte '\"'  # 48