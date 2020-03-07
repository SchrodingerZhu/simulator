# input a,b; output a/b, a%b
.text

addi $v0, $zero, 5
syscall
add $t0, $zero, $v0

addi $v0, $zero, 5
syscall
add $t1, $zero, $v0

div $t0, $t1
mfhi $t2
mflo $t3

addi $v0, $zero, 1
add $a0, $zero, $t3
syscall

addi $v0, $zero, 11
addi $a0, $zero, 32
syscall

addi $v0, $zero, 1
add $a0, $zero, $t2
syscall

addi $v0, $zero, 10
syscall