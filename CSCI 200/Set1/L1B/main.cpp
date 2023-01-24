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
		cout << "Do you want another random value? (Y/N) ";
		cin >> userInput;
	}

	return 0;
}
