/* CSCI 200: Lab 1A: Math Equation Solver
 *
 * Author: Tristen Erpelding
 *
 * Create two math eqatuions that recieve input by the user and then will output the answer to the user.
 */

// The include section adds extra definitions from the C++ standard library.
#include <iostream> // For cin, cout, etc.
#include <cmath>
#include <iomanip>

// We will (most of the time) use the standard library namespace in our programs.
using namespace std;

// Define any constants below this comment.
const float PI = 3.14159;
const float fourThird = 4/3.0;

// Must have a function named "main", which is the starting point of a C++ program.
int main() {

  /******** INSERT YOUR CODE BELOW HERE ********/
  // Create the variables and initalize them with values using float as numbers could possibly be decimals
  /* --< Equation 1 Variables >-- */
  float distance = 0.0;
  float xVal = 0.0;
  float yVal = 0.0;
  /* --< Equation 2 Variables >-- */
  float radius = 0.0;
  float volumeSphere = 0.0;

 // Get the input for the variables in Equation 1
  cout << endl;
  cout << "Equation 1" << endl;
  cout << "Enter the X Value: ";
  cin >> xVal;
  cout << "Enter the Y Value: ";
  cin >> yVal;

  // Calulate Equation 1
  distance = sqrt(pow(xVal,2) + pow(yVal,2));

  //Print the distance of Equation 1
  cout << "The distance from the X-Value to the Y-Value is: " << fixed << setprecision(3) << distance << endl;

  cout << endl;

  // Get the input for the variables in Eqation 2
  cout << "Equation 2" << endl;
  cout << "Enter the Radius: ";
  cin >> radius;

  // Calculate Equation 2
  volumeSphere = (fourThird * PI) * pow(radius,3);

  // Print the volume of a sphere for Equation 2
  cout << "The volume of a sphere with radius: " << radius << " is " << fixed << setprecision(3) << volumeSphere << endl; 

  /******** INSERT YOUR CODE ABOVE HERE ********/

  return 0; // signals the operating system that our program ended OK.
}
