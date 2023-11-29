#!/bin/bash

echo "Enter a title:"
read -r title

# Split the title into an array of words
IFS=' ' read -ra words <<< "$title"

# Initialize a variable to track whether the current word is the first word
first_word=true

# Define a list of exceptions
exceptions=("a" "an" "in" "to" "from" "for" "the" "of" "by")

# Loop through each word in the title
for word in "${words[@]}"; do
    # Get the first letter of the current word
    first_letter=${word:0:1}

    # Check if the current word is an exception
    is_exception=false
    for exception in "${exceptions[@]}"; do
        if [ "${word,,}" == "$exception" ]; then
            is_exception=true
            break
        fi
    done

    # Check capitalization based on position and exception status using case
    case $first_word in
        true)
            if [[ "${word:0:1}" =~ [a-z] ]]; then
                echo "'$word' should be capitalized."
            fi
            ;;
        false)
            if [ "$is_exception" = true ]; then
                if [[ "${word:0:1}" =~ [A-Z] ]]; then
                    echo "'$word' should NOT be capitalized."
                fi
            elif [[ "${word:0:1}" =~ [a-z] ]]; then
                echo "'$word' should be capitalized."
            fi
            ;;
    esac

    # Set the first_word variable to false after the first word
    first_word=false
done
