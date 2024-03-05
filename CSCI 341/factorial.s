# factorial.s 
# Compute n! recursively.

.data

prompt: .asciz "Enter a non-negative integer: "
endl:	.asciz "\n"

.text

.globl main

# Prompt for a non-negative integer
# and invoke the factorial function.

main:
	la	 a0, prompt
	addi a7, zero, 4
	ecall			# Display prompt.

	addi a7, zero, 5
	ecall			# Get integer in a0

	jal	ra, factorial # n is in a0
	                  # n! in a0 on return

	addi a7, zero, 1
	ecall			# Print integer result.

	la	a0, endl
	addi a7, zero, 4
	ecall			# Print endl.

    addi a7, zero, 10 # exit program
	ecall
	

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


