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

.macro store_int_array(%array,%index)
slli t1, %index, 2 # t1 = index * 4 (since each integer is 4 bytes)
add t1, t1, %array # t1 = base address of array + offset
sw a0, 0(t1) # Store the word from a0 into the address in t1
.end_macro 

.data
welcomeMsg: .asciz "Hello! Please input array integers:\n"
beforeMsg: .asciz "Value at index 5 before modification:\n"
afterMsg: .asciz "Value at index 5 before modification:\n"
.align 2 # Aligns the following data to a 4-byte boundary
array: .space 20

.text
main:
print_string(welcomeMsg)

la t6, array # Set the start of the array address to t6
addi t5, zero, 0 # Set the inital index
addi a1, zero, 6 # Set how many integers to read
addi t3, zero, 6 # Set the ending index to 6

loop:
blt t5, t3, store # If Inital index is less than ending then run "store"
j end # When the loop is finished run "end"

store:
addi a7, zero, 5  # System call number for reading a integer
ecall
store_int_array(t6,t5) # Store the integer passing the array register and the index register
addi t5, t5, 1 # Increment the index
j loop # Perfom the loop

end:
print_string(beforeMsg)

lw t4, 20(t6)
print_int(t4)

newline
print_string(afterMsg)

lw t1, 0(t6) # Get the integer at the 1st index: a[0]
lw t2, 12(t6) # Get the integer at the 3rd index: a[3]
add t3, t1, t2 # Add the two integers a[3] + a[0]
sw t3, 20(t6) # Set the 5th index to the new number: a[5] = t3

lw t4, 20(t6)
print_int(t4)

addi a7, zero, 10    # code to exit
ecall         # make the system call