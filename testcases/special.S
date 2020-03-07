# input a, output factorial(a)
.text
addi $v0, $zero, 11
lb   $a0, 48($fp)
syscall
lb   $a0, 49($fp)
syscall
lb   $a0, 50($fp)
syscall
lb   $a0, 51($fp)
syscall
lb   $a0, 52($fp)
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
lw $a0, 36($fp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($fp)
syscall
addi $v0, $zero, 1
lw $a0, 40($fp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($fp)
syscall
addi $v0, $zero, 1
lwr $a0, 44($fp)
syscall
addi $v0, $zero, 10
syscall

.data
endl: .asciiz "\n\0" # 0
fxxk1: .asciiz "\x7B\x7b\175\175\?\"" # 4   #{{}}?"
fxxk2: .asciiz "\\123\\xaf3\'7" #12    # \123\xaf3'7
fxxk3: .asciiz "0 1\t2\'  "  #24       # 0<blank>1<tab>2'
w: .word 0x123fabcd #36 306162637
k0: .word 114514     #40 114514
1k: .word -114514     # 44 114514
a: .byte 'a' # 48
b: .byte '\x62' # 49
c: .byte '\143' # 50
d: .byte '\''  # 51
e: .byte '\"'  # 52