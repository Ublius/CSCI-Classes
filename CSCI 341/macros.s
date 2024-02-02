.macro store_char(%buffer, %bufferlen)
add t0, %buffer, %bufferlen   # Calculate the address to store the character
add t1, a0, zero              # Load character from a0
sb t1, 0(t0)                  # Store the character at the calculated address
addi %bufferlen, %bufferlen, 1 # Increment the length of the string
.end_macro

.macro newline()
addi a0, zero, 10  # Set a0 to ASCII value of newline
addi a7, zero, 11  # System call number for printing a character
ecall
.end_macro

.macro get_char()
addi a7, zero, 12  # System call number for reading a character
ecall             # System call to read from stdin
.end_macro

.macro print_inti(%int) # print int immediate
addi a7, zero, 1
addi a0, zero, %int
ecall
.end_macro


.macro print_int(%reg) # print int register
addi a7, zero, 1
add a0, zero, %reg
ecall
.end_macro

.macro print_string(%reg) # print string register
addi a7, zero, 4
add a0, zero, %reg
ecall
.end_macro

.macro print_stringl(%label) # print string from label
addi a7, zero, 4
la a0, %label
ecall
.end_macro

# print char register
.macro print_char(%reg)
addi a7, zero, 11
add a0, zero, %reg
ecall
.end_macro

# print char immediate
.macro print_chari(%int)
addi a7, zero, 11
addi a0, zero, %int
ecall
.end_macro


.data
Prompt: .string "Welcome to the thunderdome."

.text

#addi t0, zero, 10 # new line ascii value

#la t1, Prompt
#print_string(t1)
#print_chari('\n')
#print_stringl(Prompt)
