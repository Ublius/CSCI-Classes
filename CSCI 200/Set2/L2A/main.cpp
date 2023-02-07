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

