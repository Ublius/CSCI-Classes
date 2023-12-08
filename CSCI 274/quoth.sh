#!/bin/bash

#######################################
# Check for quotes directory and setup menu options
if [ ! -d quotes ]
then
	echo "Error: quotes directory not found, make sure to unroll your tarball!" >&2
	exit 1
fi
cd quotes # Move into quotes directory
options=(* "Reprompt" "Exit") # Declare list
# TODO Set variable PS3 to desired prompt here
PS3="Pick a quote-sayer: "
#######################################

select var in "${options[@]}" # Syntax to loop over a list
do
	#######################################
	# Process input
	# Note: This section was written for you, no need to modify.
	# -var pulls in user-selection of the numeric menu options
	# -REPLY pulls in string user input
	# -i.e. 7) Exit
	#		-> var=7, REPLY="" if the user typed '7'
	#		-> var=, REPLY="Exit" if the user typed 'Exit'
	input=$var
	if [ -z "$var" ]
	then
		input=$REPLY
	fi
	#######################################

	# TODO: Respond to user-input. See assignment description for instructions
	# This is where most of your code will be written
	case $REPLY in
        "Exit" | "13" )
            # TODO: Step 8 - Complete the "Exit" branch
            exit 0
            ;;
        "Reprompt" | "12" )
            # TODO: Step 9 - Complete the "Reprompt" branch
            REPLY=
            ;;
        *)
            if [ -f "$input/quote.txt" ]; then
                # Display the quote
                echo "$input said..."
                cat "$input/quote.txt"
            else
                echo "Error: character not found!"
                exit 7
            fi
            ;;
    esac
done
