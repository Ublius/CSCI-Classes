#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <filename>"
	exit 1
fi

# Assign the filename provided as an argument to a variable
filename=$1

# Check if the file exists
if [ ! -e "$filename" ]; then
	echo "Error: File not found."
	exit 1
fi

# Count total characters in the file
total_characters=$(wc -c < "$filename")

# Count total characters (excluding newline characters)
characters_no_newline=$(tr -d '\n' < "$filename" | wc -m)

# Count empty lines
empty_lines=$(grep -c '^$' "$filename")

# Count alphanumeric characters
alphanumeric_count=$(tr -cd '[:alnum:]' < "$filename" | wc -m)

# Display the results
echo "This file has $total_characters characters in it."
echo "This file has $characters_no_newline characters not counting the newline."
echo "This file has $empty_lines empty lines."
echo "This file contains $alphanumeric_count alphanumeric characters."
