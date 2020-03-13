.data		#data segment starts at addr 0x00500000 (1MB for text segment)
str1: .asciiz "Welcome to this grading mini program!\n"		#at 0x00500000
str2: .asciiz "Good bye!"					#at 0x00500028
str3: .asciiz "Please enter the student number (1-10):"		#at 0x00500034
str4: .asciiz "Please enter the grade (0-100):"			#at 0x0050005C
grade: .word 0,0,0,0,0,0,0,0,0,0				#at 0x0050007C
str5: .asciiz "The grade is now:"				#at 0x005000A4
str6: .asciiz "at address:"					#at 0x005000B8
str7: .asciiz "of student:"					#at 0x005000C4


.text
main:	lui $a0, 80				#load str1 addr to $a0 and print.
	addi $v0, $zero, 4
	syscall
	lui $a0, 80				#load str3 addr to $a0 and print.
	ori $a0, $a0, 52
	addi $v0, $zero, 4
	syscall
	addi $v0, $v0, 1			#$v0 has 5, read int.
	syscall
	add $a0, $zero, $v0
	jal func

	addi $t1, $v0, 0			#transfer the student number to $t1
	addi $t0, $v1, 0			#transfer the grade addr to $t0
	lui $a0, 80				#load str5 addr and print
	ori $a0, $a0, 164
	addi $v0, $zero, 4
	syscall
	lw $a0, 0($t0)				#load the grade to $a0 and print
	addi $v0, $zero, 1
	syscall
	lui $a0, 80				#load str7 addr and print
	ori $a0, $a0, 196
	addi $v0, $zero, 4
	syscall
	add $a0, $zero, $t1			#print student number
	addi $v0, $zero, 1
	syscall
	lui $a0, 80				#load str6 addr and print
	ori $a0, $a0, 184
	addi $v0, $zero, 4
	syscall
	add $a0, $zero, $t0			#print grade addr
	addi $v0, $zero, 1
	syscall
	lui $a0, 80				#load str2 addr and print
	ori $a0, $a0, 40
	addi $v0, $zero, 4
	syscall
	addi $v0, $zero, 10			#exit
	syscall

func:	addi $sp, $sp, -4
	sw $v0, 0($sp)
	add $t0, $zero, $a0			#$a0 originally holds the student number, transfer to $t0
	lui $a0, 80					#put addr of str4 into $a0
	ori $a0, $a0, 92
	addi $v0, $zero, 4			#print str4
	syscall
	addi $v0, $v0, 1			#$v0 has 5, read int.
	syscall
	add $t1, $zero, $v0			#$t1 stores student grade
	lui $t2, 80					#load the start of the grade array into $t2
	ori $t2, $t2, 124
	addi $t3, $zero, 1			#put loop counter in $t1
loop:	beq $t0, $t3, end
	addi $t2,$t2, 4
	addi $t3, $t3, 1			#increment $t3
	j loop
end:	sw $t1, 0($t2)				#put grade in place
	add $v1, $t2, $zero			#return the address of the changed grade
	lw $v0, 0($sp)				#restore original $v0
	addi $sp, $sp, 4
	jr $ra
