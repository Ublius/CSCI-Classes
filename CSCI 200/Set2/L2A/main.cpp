/* CSCI 200: Lab 2: L2A - The Secret Moosage
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     Looked up the notes on how to input a file as well as output to a file.
 * 	   Bethany showed me that you could easily convert a character to an integer add one then convert back.
 * 
 * 	   A program that reads the ciphered text file and then writes a deciphered version to a file called decipheredMessage.txt. For each character in the file, your program should implement the following replacement algorithm:
    	If the character read is a newline character '\n', then you should write the newline character to the file.
    	Otherwise, if the character read is a ~, you should write a space to the file.
    	Otherwise, write the character read "offset" by +1.
 */

#include <fstream>
#include <iostream>
using namespace std;

int main() {
	// declare ofstream object and open the file
	ifstream inFile("secretMessage.txt");
	ofstream outFile("decipheredMessage.txt");

	if(inFile.fail()) // Check to make sure the file can be opened
	{
		cerr << "Error opening the input file" << endl;
		return -1;
	}

	if (outFile.fail()) // Check to make sure the file can be opened
	{
		cerr << "Error opening output file" << endl;
		return -1;
	}

	string word = "";
	char chr;
	while(!inFile.eof()) // Make sure it is not the end of the file
	{
		while(inFile.get(chr)) // Get each character
		{
			if(chr == '\n') // If end of line make \n in output
				outFile << endl;
			else if (chr == '~') // Check if ~ and make " "
				outFile << " ";
			else
			{
				chr = (int)chr + 1; // Change the letter to the next letter
				outFile << (char)chr;
			}
		}
	}
	
	// Close the files
	inFile.close();
	outFile.close();
	return 0;
}

