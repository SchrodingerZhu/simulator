		addi $v0, $zero, 5
		syscall
		add $s0, $v0, $zero
		add $s1, $zero, $zero
		addi $s2, $zero, 0
		addi $s3, $zero, 1
loop:	addi $s1, $s1, 1
		add $s4, $s3, $s2
		addi $s3, $s2, 0
		addi $s2, $s4, 0
		bne $s1, $s0, loop
		addi $v0, $zero, 1
		addi $a0, $s4, 0
		syscall