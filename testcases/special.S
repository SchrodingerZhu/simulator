# input a, output factorial(a)
.text
addi $v0, $zero, 11
lb   $a0, 48($gp)
syscall
lb   $a0, 49($gp)
syscall
lb   $a0, 50($gp)
syscall
lb   $a0, 51($gp)
syscall
lb   $a0, 52($gp)
syscall
lb   $a0, 0($gp)
syscall

addi $v0, $zero, 4
addi $a0, $gp, 4
syscall
addi $a0, $gp, 0
syscall

addi $a0, $gp, 12
syscall
addi $a0, $gp, 0
syscall

addi $a0, $gp, 24
syscall
addi $a0, $gp, 0
syscall

addi $v0, $zero, 1
lw $a0, 36($gp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($gp)
syscall
addi $v0, $zero, 1
lw $a0, 40($gp)
syscall
addi $v0, $zero, 11
lbu   $a0, 0($gp)
syscall
addi $v0, $zero, 1
lwr $a0, 44($gp)
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