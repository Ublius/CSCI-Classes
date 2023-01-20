#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
	srand(time(0));
	cout << rand() << endl;
	cout << RAND_MAX << endl;

	// Generate a random number between 2 & 10 inlusive
	// rand() % (max - min + 1) + min
	
	// Generate s random float between 2 & 10 inclusive
	// rand() / (double) RAND_MAX * (max - min) + min
}
