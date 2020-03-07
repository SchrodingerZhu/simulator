# input a and n and then output a ^ n
.text
    addi $v0, $zero, 5
    syscall
    add $a0, $zero, $v0

    addi $v0, $zero, 5
    syscall
    add $a1, $zero, $v0

    jal POW

    add $a0, $zero, $v0
    addi $v0, $zero, 1
    syscall

    addi $v0, $zero, 10
    syscall
POW:
    addi $sp, $sp, -12
    sw  $a1, 8($sp)
    sw  $a0, 4($sp)
    sw  $t0, 0($sp)
    addi $v0, $zero, 1
    # while (n != 0)
__POW_LOOP_START:
    bne $zero, $a1, __POW_BRANCH_1
    lw  $a1, 8($sp)
    lw  $a0, 4($sp)
    lw  $t0, 0($sp)
    addi $sp, $sp, 12
    jr $ra
__POW_BRANCH_1:
    andi $t0, $a1, 1
    beq $zero, $t0, __POW_BRANCH_2
    mul $v0, $v0, $a0
__POW_BRANCH_2:
    srl $a1, $a1, 1
    mul $a0, $a0, $a0
    j __POW_LOOP_START

