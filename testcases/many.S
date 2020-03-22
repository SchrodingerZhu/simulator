.data		#data segment starts at addr 0x00500000 (1MB for text segment)
str1: .asciiz "Testing lb,sb,read/print_char,sbrk"			#at 0x00500000
str2: .asciiz "Please enter a char:"				#at 0x00500024
str3: .asciiz "The char you entered is:"			#at 0x0050003C
str4: .asciiz "Testing for .ascii"				#at 0x00500058
str5: .ascii "aaaa"						#at 0x0050006C
str6: .ascii "bbbb"						#at 0x00500070
str7: .asciiz "ccc"						#at 0x00500074
str8: .asciiz "Testing for fileIO syscalls"				#at 0x00500078
str9: .asciiz "/tmp/file.txt"				#at 0x00500094
str10: .asciiz "If you see this, your fileIO is all cool"	#at 0x005000A4
str11: .asciiz "num of chars printed to file:"				#at 0x005000D0
str12: .asciiz "Goodbye"			#at 0x005000F0
str13: .asciiz "You should see aaaabbbbccc, bbbbccc, ccc for three strings"	#at 0x005000F8
half: .half 1,2						#at 0x00500134
byte: .byte 1,2,3,4					#at 0x00500138
str14: .asciiz "Testing for .half,.byte"	#at 0x0050013C
str15: .asciiz "For half, the output should be: 65539 in decimal, and you have:"	#at 0x00500154
str16: .asciiz "For byte, the output should be: 16909059 in decimal, and you have:"	#at 0x00500194


.text
main:	addi $a0,$a0, 80			#load str1 addr to $a0 and print.
	sll $a0,$a0,16
	addi $v0, $zero, 4
	syscall

	lui $a0, 80				#load str2 addr to $a0 and print.
	ori $a0, $a0, 36
	addi $v0, $zero, 4
	syscall

	addi $v0, $v0, 8			#$v0 has 12, read char.
	syscall
	add $t0, $zero, $v0			#char read now in $t0

	addi $v0, $zero, 9			#calling sbrk
	addi $a0, $zero, 4
	syscall
	add $t1, $zero, $v0
	sb $t0, 0($t1)

	lui $a0, 80				#load str3 addr to $a0 and print.
	ori $a0, $a0, 60
	addi $v0, $zero, 4
	syscall

	lb $a0, 0($t1)
	addi $v0, $v0, 7			#print char
	syscall



############################################


	addi $a0,$zero, 80			#print str4
	sll $a0, $a0, 20
	srl $a0, $a0, 4
	ori $a0, $a0, 88
	addi $v0, $zero, 4
	syscall


	lui $a0, 80				#print str5
	ori $a0, $a0, 108
	addi $v0, $zero, 4
	syscall

	lui $a0, 80				#print str6
	ori $a0, $a0, 112
	addi $v0, $zero, 4
	syscall

	lui $a0, 80				#print str7
	ori $a0, $a0, 116
	addi $v0, $zero, 4
	syscall

	lui $a0, 80				#print str13
	ori $a0, $a0, 248
	addi $v0, $zero, 4
	syscall

############################################

	lui $a0, 80				#print str8
	ori $a0, $a0, 120
	addi $v0, $zero, 4
	syscall

	lui $a0, 80				#open file
	ori $a0, $a0, 148
	addi $a1, $zero, 2
	addi $v0, $v0, 9
	syscall

	add $t0, $a0, $zero		#transfer the file descriptor to $t0

	addi $t5, $zero, 4
	sub $v0, $v0, $t5			#sbrk
	addi $a0, $zero, 60
	syscall

	add $t1, $zero, $v0	#transfer the allocated mem to $t1

	add $a0, $t0, $zero		#write str10 to file
	lui $a1, 80
	ori $a1, $a1, 164
	addi $a2, $zero, 41
	addi $v0, $zero, 15
	syscall

	add $t2, $zero, $a0 	#transfer the num of chars written to $t2

	lui $a0, 80				#print str11
	ori $a0, $a0, 208
	addi $v0, $zero, 4
	syscall

	add $a0, $zero, $t2	#print num of chars written to file
	addi $t5, $zero, 3
	sub $v0, $v0, $t5
	syscall

	addi $v0, $zero, 16		#close file
	add $a0, $zero, $t0
	syscall

	lui $a0, 80				#open the file again
	ori $a0, $a0, 148
	addi $a1, $zero, 2
	addi $v0, $zero, 13
	syscall

	addi $a0, $t0, 0		#read from the file
	addi $a1, $t1, 0
	addi $a2, $zero, 41
	addi $v0, $zero, 14
	syscall

	addi $v0, $zero, 16		#close file
	add $a0, $zero, $t0
	syscall

	add $a0, $zero, $t1		#print the content read from file
	addi $v0, $zero, 4
	syscall





############################################
	lui $a0, 80				#print str14
	ori $a0, $a0, 316
	addi $v0, $zero, 4
	syscall

	lui $t0, 80				#load half array
	ori $t0, $t0, 308


	lui $a0, 80				#print str15
	ori $a0, $a0, 340
	addi $v0, $zero, 4
	syscall

	lh $a0, 0($t0)
	sll $a0, $a0, 16
	lh $t2, 2($t0)
	or $a0, $a0,$t2

	addi $a0, $a0, 1
	addi $v0, $zero, 1
	syscall


	lui $a0, 80				#print str16
	ori $a0, $a0, 404
	addi $v0, $zero, 4
	syscall


	lui $t1, 80				#load byte array
	ori $t1, $t0, 312

	lb $t2, 0($t1)
	sll $t2, $t2, 24
	lb $t3, 1($t1)
	sll $t3, $t3, 16
	lb $t4, 2($t1)
	sll $t4, $t4, 8
	lb $t5, 3($t1)

	or $a0, $t2,$t3
	or $a0, $a0,$t4
	or $a0, $a0,$t5

	addi $a0, $a0, -1
	addi $v0, $zero, 1
	syscall


############################################



	lui $a0, 80				#print str12
	ori $a0, $a0, 240
	addi $v0, $zero, 4
	syscall

	addi $v0, $zero, 10			#exit
	syscall


