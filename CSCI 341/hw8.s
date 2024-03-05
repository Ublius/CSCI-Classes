.macro newline
addi a0, zero, 10  # Set a0 to ASCII value of newline
addi a7, zero, 11  # System call number for printing a character
ecall
.end_macro

.macro print_string(%label) # print string from label
addi a7, zero, 4
la a0, %label
ecall
.end_macro

.macro print_int(%reg) # print int register
addi a7, zero, 1
add a0, zero, %reg
ecall
.end_macro


.data
firstMsg: .asciz "How many objects can you choose from: "
secondMsg: .asciz "How many objects will you be choosing from: "
resultMsg1st: .asciz "The result of C("
commaMsg: .asciz ","
resultMsg2nd: .asciz ") is "
errorMsg: .asciz "Error: Negative value entered."

#s0 = n
#s1 = k
#s2 = n-k
#s3 = n!
#s4 = k!
#s5 = (n-k)!
#s6 = k!*(n-k)!
#s7 = n!/k!(n-k)!

.text
main:
print_string(firstMsg)	# Display prompt.

addi a7, zero, 5
ecall	# Get integer in a0

add s0, zero, a0 #Store n into s0; s0 = n

print_string(secondMsg)	# Display prompt.

addi a7, zero, 5
ecall	# Get integer in a0

add s1, zero, a0 #Store k into s1; s1 = k

blt s0, zero, error # Check if n is negative
blt s1, zero, error # Check if k is negative

sub s2, s0, s1 # s2 = n -k
blt s2, zero, nCk0 # if n-k < 0

add a0, zero, s0
jal ra, factorial # n is in a0
# n! in a0 on return
add s3, zero, a0 #s3 = n!

add a0, zero, s1
jal ra, factorial # k is in a0
# k! in a0 on return
add s4, zero, a0 #s3 = k!

add a0, zero, s2
jal ra, factorial # n-k is in a0
# (n-k)! in a0 on return
add s5, zero, a0 #s3 = (n-k)!

mul s6, s4, s5 #s6 = k!*(n-k)!
div s7, s3, s6 #s7 = n!/k!(n-k)!

beq zero, zero, result # Go to the result

#.globl factorial
# Preconditions:	
#   1st parameter (a0) non-negative integer, n
# Postconditions:
#   result (a0) n factorial
factorial:
    addi sp, sp, -8  # make space on stack
    sw   ra, 4(sp)   # save return address 
    sw   a0, 0(sp)   # save n

    addi t0, zero, 1 # 1 for comparing
    blt  t0, a0, L1  # if 1 < n need to recurse

    addi a0, zero, 1 # otherwise return 1
    addi sp, sp, 8   # release stack
    jalr zero, ra, 0 # return to caller

L1: addi a0, a0, -1  # compute n-1
    jal ra, factorial  # recurse fact(n-1)
	                 # (n-1)! is in a0

    lw t0, 0(sp)     # recover n
    lw ra, 4(sp)     # restore ra
    addi sp, sp, 8   # release stack
    mul a0, t0, a0   # compute n * fact(n-1)
    jalr zero, ra, 0 # return to caller
    
error:
print_string(errorMsg)
beq zero, zero, end

nCk0:
print_string(resultMsg1st)
print_int(s0) # n
print_string(commaMsg)
print_int(s1) # k
print_string(resultMsg2nd)
print_int(zero) # zero
beq zero, zero, end


result:
print_string(resultMsg1st)
print_int(s0) # n
print_string(commaMsg)
print_int(s1) # k
print_string(resultMsg2nd)
print_int(s7) # n!/k!*(n-k)!
beq zero, zero, end

end:
addi a7, zero, 10    # code to exit
ecall         # make the system call