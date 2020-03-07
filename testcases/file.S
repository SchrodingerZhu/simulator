# read two alsolute path and copy content of input to output
.text
    addi $v0, $zero, 9
    addi $a0, $zero, 1024
    syscall
    add $t0, $zero, $v0 # t0: read path memory

    addi $v0, $zero, 9
    addi $a0, $zero, 1024
    syscall
    add $t1, $zero, $v0 # t1: write path memory

    addi $v0, $zero, 9
    addi $a0, $zero, 2048
    syscall
    add $t2, $zero, $v0 # t2: file memory

    addi $v0, $zero, 8
    addi $a0, $t0,   0
    addi $a1, $zero, 1000 # read path
    syscall

    addi $v0, $zero, 8
    addi $a0, $t1,   0
    addi $a1, $zero, 1000 # write path
    syscall

    addi $v0, $zero, 13
    addi $a0, $t0,   0 # read path
    addi $a1, $zero, 0 # read only
    addi $a2, $zero, 3 # access mode
    syscall
    add $t3, $zero, $v0 # t3: fd

    addi $v0, $zero, 14
    addi $a0, $t3,   0 # fd
    addi $a1, $t2,   0   # read buffer
    addi $a2, $zero, 2000 # read length
    syscall

    addi $v0, $zero, 4   # print file
    addi $a0, $t2,   0   # buffer
    syscall

    addi $v0, $zero, 13
    addi $a0, $t1,   0 # write path
    addi $a1, $zero, 4011005 # write_only | sync | trunc | creat
    addi $a2, $zero, 3 # access mode
    syscall
    add $t4, $zero, $v0 # t4: fd

    addi $v0, $zero, 15 # write
    addi $a0, $t4,   0 # fd
    addi $a1, $t2,   0   # buffer
    addi $a2, $zero, 2048 # write length
    syscall

    addi $v0, $zero, 16 # close
    addi $a0, $t4,   0 # fd
    syscall

    addi $v0, $zero, 16 # close
    addi $a0, $t3,   0 # fd
    syscall

    addi $v0, $zero, 10 # exit
    syscall



