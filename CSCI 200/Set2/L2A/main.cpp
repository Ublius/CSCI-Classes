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

	if(inFile.fail())
	{
		cerr << "Error opening the input file" << endl;
		return -1;
	}

	if (outFile.fail()) 
	{
		cerr << "Error opening output file" << endl;
		return -1;
	}

	string word = "";
	char chr;
	while(!inFile.eof())
	{
		while(inFile.get(chr))
		{
			if(chr == '\n')
				outFile << endl;
			else if (chr == '~')
				outFile << " ";
			else
			{
				chr = (int)chr + 1;
				outFile << (char)chr;
			}
		}
	}
	
	inFile.close();
	outFile.close();
	return 0;
}

