.macro print_string(%label) # print string from label
addi a7, zero, 4
la a0, %label
ecall
.end_macro

.macro store_char(%buffer, %bufferlen)
add t0, %buffer, %bufferlen   # Calculate the address to store the character
add t1, a0, zero              # Load character from a0
sb t1, 0(t0)                  # Store the character at the calculated address
addi %bufferlen, %bufferlen, 1 # Increment the length of the string
.end_macro

.macro get_char()
addi a7, zero, 12  # System call number for reading a character
ecall             # System call to read from stdin
.end_macro

.macro newline()
addi a0, zero, 10  # Set a0 to ASCII value of newline
addi a7, zero, 11  # System call number for printing a character
ecall
.end_macro

.data
userMsg: .space 6 # Set a space in mem for 5 characters & null term
welcomeMsg: .asciz "Hello! Please enter 5 characters:\n"
endingMsg: .asciz "You entered: "

.text
main:
la a3, userMsg # Load address of the buffer
addi a4,zero,0 # Set Length of the string

print_string(welcomeMsg)

# Char 1
get_char()
store_char(a3,a4)
newline()
# Cha 2
get_char()
store_char(a3,a4)
newline()
# Cha 3
get_char()
store_char(a3,a4)
newline()
# Cha 4
get_char()
store_char(a3,a4)
newline()
# Cha 5
get_char()
store_char(a3,a4)
newline()

# Final Message
print_string(endingMsg)
print_string(userMsg)

addi a7, zero, 10    # code to exit
ecall         # make the system call
