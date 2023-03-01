#include "string_functions.h"

unsigned long string_length(const string STR)  {
    unsigned long result = -1;
    // set result to the length of the string
    result = STR.length();  
    return result;
}

char string_char_at(const string STR, const int IDX) {
    char result = '\0';
    // set result to the character of a string at a given index
    for(int i = 0; i < STR.length(); i++)
    {
        if(i == IDX)
            result = STR[i];
    }
    return result;
}

string string_append(const string LEFT, const string RIGHT)  {
    string result = LEFT;
    // set result to the concatenation of strings LEFT and RIGHT
    result = LEFT + RIGHT;
    return result;
}

string string_insert(const string STR, const string TO_INSERT, const int IDX) {
    string result = STR;
    // set result to the result of inserting a string into another
    result.insert(IDX, TO_INSERT);
    return result;
}

size_t string_find(const string STR, const char C)  {
    size_t result = 0;
    // set result to the index of the first occurrence of the character
    for(int i = 0; i < STR.length(); i++)
    {
        if((char)STR[i] == C)
        {
            result = i;
            return result;
        }
    }
    return string::npos;
}

string string_substring(const string STR, const int IDX, const int LEN) {
    string result = STR;
    // set result to be a substring starting at index of given length
    result = STR.substr(IDX, LEN);
    return result;
}

string string_replace(const string STR, const string TEXT_TO_REPLACE, const string REPLACE_WITH) {
    string result = STR;
    int i;
    // set result to the string with the string replaced
    if(STR.find(TEXT_TO_REPLACE) != string::npos) 
    {
        i = STR.find(TEXT_TO_REPLACE);
        result.replace(i, TEXT_TO_REPLACE.length(), REPLACE_WITH);
    }
    return result;
}

string string_first_word(const string STR)  {
    string result = STR;
    // set result to the first word from the string
    int i = STR.find(' ');
    result = STR.substr(0, i);
    return result;
}

string string_remove_first_word(const string STR)  {
    string result = STR;
    // set result to be the string with the first word removed
    if(STR.find(' ') != string::npos)
    {
        int i = STR.find(' ');
        result = STR.substr(i+1);
    }
    else
        result = "";

    return result;
}

string string_second_word(const string STR)  {
    string result = "";
    // set result to be the second word from the string
    int count = 0;
    for(int i = 0; i < STR.length(); i++)
    {
        if(STR[i] != ' ')
            result += STR[i];
        else
        {
            count++;
            if(count == 2)
                return result;
            result = "";
        }
    }

    if (count < 1 && !result.empty())
        result = "";

    return result;
}

string string_third_word(const string STR)  {
    string result = "";
    // set result to be the third word from the string
    int count = 0;
    for(int i = 0; i < STR.length(); i++)
    {
        if(STR[i] != ' ')
            result += STR[i];
        else
        {
            count++;
            if(count == 3)
                return result;
            result = "";
        }
    }

    if (count < 2 && !result.empty())
        result = "";

    return result;
}

string string_nth_word(const string STR, const int N)  {
    string result = "";
    // set result to be the nth word from the string
    int count = 0;
    for(int i = 0; i < STR.length(); i++)
    {
        if(STR[i] != ' ')
            result += STR[i];
        else
        {
            count++;
            if(count == N)
                return result;
            result = "";
        }
    }

    if (count < N-1 && !result.empty())
        result = "";

    return result;
}

vector<string> string_tokenize(const string STR, const char DELIMINATOR) {
    vector<string> result;
    // split the string by the given deliminator
    size_t start = 0;
    size_t end = STR.find(DELIMINATOR);
    while (end != string::npos) 
    {
        result.push_back(STR.substr(start, end - start));
        start = end + 1;
        end = STR.find(DELIMINATOR, start);
    }
    result.push_back(STR.substr(start, end));
    return result;
}

string string_substitute(const string STR, const char TARGET, const char REPLACEMENT)  {
    string result = STR;
    // set result to be the string with all instances of TARGET replaced
    for(int i = 0; i < result.length(); i++)
    {
        if(result[i] == TARGET)
            result[i] = REPLACEMENT;
    }
    return result;
}

string string_to_lower(const string STR) {
    string result;
    // convert all characters to lower case
    for(int i = 0; i < STR.length(); i++)
    {
        result += tolower(STR[i]);
    }
    return result;
}

string string_to_upper(const string STR) {
    string result;
    // convert all characters to upper case
        for(int i = 0; i < STR.length(); i++)
    {
        result += toupper(STR[i]);
    }
    return result;
}

int string_compare(const string LHS, const string RHS) {
    int result = 0;
    // TODO 16: compare LHS and RHS
    if(LHS == RHS)
        result = 0;
    else if (LHS < RHS)
        result = -1;
    else if (LHS > RHS)
        result = 1;

    return result;
}