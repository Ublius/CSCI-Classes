/* CSCI 200: Lab 1B: L1B - Random Classification
 *
 * Author: Tristen Erpelding
 *
 * Have the user enter a minimum and maximum value that then a random float is generated inclusive of those numbers. Print which quartile the number is in. Keep prompting the user if the want a new random number. 
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

/* --< Func to gernate random value >-- */
float randomValue(float min, float max)
{
	return rand() / (double)RAND_MAX * (max - min) + min;
}

/* --< Func to get the quartile >-- */
void findQuartile(float min, float max, float randomValue)
{
	float median = ((max - min) / 2) + min;
	float q1 = ((median - min) / 2) + min;
	float q3 = ((max - median) / 2) + median;
	// random <= q1
	if(randomValue <= q1) {
		cout << "This is in the first quartile" << endl;
	}
	else if (randomValue <= median) {
		cout << "This is in the second quartile" << endl;
	}
	// random <= q3
	else if (randomValue <= q3) {
		cout << "This is in the third quartile" << endl;
	}
	// else
	else {
		cout << "This is in the fourth quartile" << endl;
	}
}

int main() {
	srand(time(0)); //rand is now seeded
	rand(); //throw away frist value

	float minValue = 0.0f;
	float maxValue = 0.0f;
	float randValue = 0.0f;
	char userInput = 'y';

	/* --< Prompt the user for input >-- */
	cout << "Enter the minimum value: ";
	cin >> minValue;
	cout << "Etner the maximum value: ";
	cin >> maxValue;
	
	while(userInput == 'y')
	{
		randValue = randomValue(minValue, maxValue);
		cout << "A random value is: " << fixed << setprecision(2) << randValue << endl;
		findQuartile(minValue, maxValue, randValue);
		cout << "Do you want another random value? (Y/N) ";
		cin >> userInput;
	}
	
	cout << "Hace a nice day!" << endl;

	return 0;
}
