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
welcomeMsg: .asciz "Hello! Please input an integer:\n"
outputMsg: .asciz "X value:\n"

.text
main:
print_string(welcomeMsg)

addi a7, zero, 5 # Get integer input
ecall

add t0, zero, a0 # Set t0 to the inputed intger
addi t1, zero, 64 # Set t1 to 64

blt t1, t0, xgte64 # x > 64
bge t1, t0, xlessthaneq64 # x <= 64

xgte64:
jal end

xlessthaneq64:
addi t1, zero, 32 # Set t1 to 32
bge t1, t0, xless32 # x <= 32
srai t0, t0, 2 # x/4
jal end

xless32:
addi t3, zero, 2 # set t3 to 2
rem t2, t0, t3 # x % 2
bne t2, zero, else # x%2 != 0
slli t0, t0, 1 # x * 2
jal end

else:
addi t0, t0, 7 # x + 7
jal end

end:
print_string(outputMsg)
print_int(t0) # Print x
addi a7, zero, 10    # code to exit
ecall         # make the system call
