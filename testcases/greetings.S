# greetings
# input your name and then output greetings
.text
addi $v0, $zero, 4
add $a0, $zero, $fp
syscall
addi $v0, $zero, 9
addi $a0, $zero, 1024
syscall
add  $t0, $zero, $v0  # t0: free memory
addi $v0, $zero, 8 # read string
add $a0, $zero, $t0 # string address
addi $a1, $zero, 12 # string length 12
syscall
addi $v0, $zero, 4
addi $a0, $fp, 16 # print "hello, "
syscall
addi $a0, $t0, 0 # print name
syscall
addi $v0, $zero, 10 # exit with 0
syscall
.data
first: .asciiz "hello, world\n" # 0-13
second: .asciiz "hello, " # 16
