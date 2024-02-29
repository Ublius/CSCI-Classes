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

.macro load_int_array(%array, %index)
    slli t1, %index, 2       # t1 = index * 4 (since each integer is 4 bytes)
    add t1, t1, %array       # t1 = base address of array + offset
    lw a0, 0(t1)             # Load the word from the address in t1 into a0
.end_macro

.data
firstArrayMsg: .asciz "Enter the size of the first array: "
secondArrayMsg: .asciz "Enter the size of the second array: "
enterMsg: .asciz "Enter the numbers below:\n"
searchMsg: .asciz "Enter a value to search for: "
foundFirstPirntMsg: .asciz "In the first array, the value is at index "
foundSecondPrintMsg: .asciz "In the second array, the value is at index "
notFoundFirstPrintMsg: .asciz "In the first array, the value was not found."
notFoundSecondPrintMsg: .asciz "In the second array, the value was not found."
.align 2 # Aligns the following data to a 4-byte boundary
array1: .space 40
array2: .space 40

.text
main:

firstArray:
print_string(firstArrayMsg)

addi a7, zero, 5  # System call number for reading a int
ecall             # System call to read from stdin

add s1, zero, a0 # Set the length of the first array

print_string(enterMsg)

la t6, array1 # Set the start of the frist array address to t6
addi t5, zero, 0 # Set the inital index
add a1, zero, s1  # Set how many integers to read
add t3, zero, s1 # Set the ending index to length of array

loopFirst:
blt t5, t3, storeFirst # If Inital index is less than ending then run "storeFirst"
jal zero, secondArray # When the loop is finished run "end"

storeFirst:
addi a7, zero, 5  # System call number for reading a integer
ecall
store_int_array(t6,t5) # Store the integer passing the array register and the index register
addi t5, t5, 1 # Increment the index
jal zero, loopFirst # Perfom the loop

secondArray:
print_string(secondArrayMsg)

addi a7, zero, 5  # System call number for reading a int
ecall             # System call to read from stdin

add s2, zero, a0 # Set the length of the second array

print_string(enterMsg)

la t2, array2 # Set the start of the second array address to t2
addi t5, zero, 0 # Set the inital index
add a1, zero, s2  # Set how many integers to read
add t3, zero, s2 # Set the ending index to length of array

loopSecond:
blt t5, t3, storeSecond # If Inital index is less than ending then run "storeSecond"
jal zero, searchValue # When the loop is finished run "end"

storeSecond:
addi a7, zero, 5  # System call number for reading a integer
ecall
store_int_array(t2,t5) # Store the integer passing the array register and the index register
addi t5, t5, 1 # Increment the index
jal zero, loopSecond # Perfom the loop


searchValue:
print_string(searchMsg)
addi a7, zero, 5  # System call number for reading a int
ecall             # System call to read from stdin
add t4, zero, a0 # Stores the Value to find

add t5, zero, zero # Set the inital index
add a0, zero, zero # Set the comparison register to 0
# Serarch the first array
loopFindFirst: 
beq t4, a0, foundFirst # if value = array1[index] go to found
beq t5, s1, notFindFirst # if index reaches end go to not found
load_int_array(t6, t5) # Load the integer at index position
addi t5, t5, 1 # Increment the index
jal zero, loopFindFirst # Start loop again

foundFirst:
print_string(foundFirstPirntMsg)
addi t5, t5, -1  # Decrement the index to get the correct value
blt t5, zero, negIndexFirst
print_int(t5) # Pirnt the index
newline
jal zero, findSecond

notFindFirst:
print_string(notFoundFirstPrintMsg)
newline
jal zero, findSecond

findSecond:
add t5, zero, zero # Set the inital index
add a0, zero, zero # Set the comparison register to 0

loopFindSecond:
beq s2, zero, notFindSecond
beq t4, a0, foundSecond # if value = array2[index] go to found
beq t5, s2, notFindSecond # if index reaches end go to not found
load_int_array(t2, t5) # Load the integer at index position
addi t5, t5, 1 # Increment the index
jal zero, loopFindSecond # Start loop again

foundSecond:
print_string(foundSecondPrintMsg)
addi t5, t5, -1  # Decrement the index to get the correct value
blt t5, zero, negIndexSecond
print_int(t5) # Pirnt the index
jal zero, endFunc

negIndexFirst:
print_int(zero) # Pirnt the index
newline
jal zero, findSecond

negIndexSecond:
print_int(zero) # Pirnt the index
jal zero, endFunc

notFindSecond:
print_string(notFoundSecondPrintMsg)
jal ra, endFunc


endFunc:
jal ra, end

addi a7, zero, 10    # code to exit
ecall         # make the system call

end:
jalr zero, ra, 0