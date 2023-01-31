/* CSCI 200: Assignment 1: A1 - Rock Paper Scissor Throw Down!
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 *
 * Play a game of Rock, Paper, Scissors where the user input the choice and they play against the computer. Keep track of wins, losses, and ties.
 */

// The include section adds extra definitions from the C++ standard library.
#include <iostream> // For cin, cout, etc.
#include <cstdlib>
#include <ctime>

// We will (most of the time) use the standard library namespace in our programs.
using namespace std;

// Define any constants below this comment.
const int rock = 1;
const int paper = 2;
const int scissors = 3;

/* --< Functions >-- */
int getAssocNum(char userChoice)
{
  if(userChoice == 'r' || userChoice == 'R')
  {
    cout << "Player choose Rock" << endl;
    return rock;
  }
  else if(userChoice == 'p' || userChoice == 'P')
  {
    cout << "Player choose Paper" << endl;
    return paper;
  }
  else if(userChoice == 's' || userChoice == 'S')
  {
    cout << "Player choose Scissors" << endl;
    return scissors;
  }

  return 0;
}

int getCompValue(int max, int min)
{
  int num = rand() % (max - min + 1) + min;

  if(num == 1)
    cout << "Computer choose Rock" << endl;
  else if(num == 2)
    cout << "Computer choose Paper" << endl;
  else if(num == 3)
    cout << "Computer choose Scissors" << endl;

  return num;
}

char calcWinner(int userChoiceNum, int compVal)
{
  cout << endl;
  if(userChoiceNum == 3)
  {
    if(userChoiceNum == compVal)
    {
      cout << "Scissors ties with scissors" << endl;
      cout << "No one wins!" << endl;
      return 't';
    }
    else if(compVal == 2)
    {
      cout << "Scissors beats paper" << endl;
      cout << "Player Wins!" << endl;
      return 'w';
    }
    else if(compVal == 1)
    {
      cout << "Rock beats scissors" << endl;
      cout << "Computer Wins!" << endl;
      return 'l';
    }
  }
  else if(userChoiceNum == 2)
  {
    if(userChoiceNum == compVal)
    {
      cout << "Paper ties with paper" << endl;
      cout << "No one wins!" << endl;
      return 't';
    }
    else if(compVal == 3)
    {
      cout << "Scissors beats paper" << endl;
      cout << "Computer Wins!" << endl;
      return 'l';
    }
    else if(compVal == 1)
    {
      cout << "Paper beats rock" << endl;
      cout << "Player Wins!" << endl;
      return 'w';
    }
  }
  else if(userChoiceNum == 1)
  {
    if(userChoiceNum == compVal)
    {
      cout << "Rock ties with Rock" << endl;
      cout << "No one wins!" << endl;
      return 't';
    }
    else if(compVal == 3)
    {
      cout << "Rock beats scissors" << endl;
      cout << "Player Wins!" << endl;
      return 'w';
    }
    else if(compVal == 2)
    {
      cout << "Paper beats rock" << endl;
      cout << "Computer Wins!" << endl;
      return 'l';
    }
  }

  return 'e';
}

// Must have a function named "main", which is the starting point of a C++ program.
int main() {
  /* --< Variables >-- */
  char continueGame = 'y';

  char gameResult = ' ';
  int wonVal = 0;
  int lostVal = 0;
  int tieVal = 0;

  char userChoice = ' ';
  int userChoiceNum = 0;

  int maxVal = 3;
  int minVal = 1;
  int compVal = 0;

  srand(time(0)); //rand is now seeded
  rand(); //throw away frist value

  /******** INSERT YOUR CODE BELOW HERE ********/
  while(continueGame == 'y' || continueGame == 'Y')
  {
    // Get the player choice
    cout << "Welcome one and all to a round of Rock, Paper, Scissors! (Enter P, R or S)" << endl;
    cout << "Player: ";
    cin >> userChoice;
    cout << endl;
    
    // Peform operations to covert player choice, get computer choice, and get game result
    userChoiceNum = getAssocNum(userChoice);
    compVal = getCompValue(maxVal, minVal);
    gameResult = calcWinner(userChoiceNum, compVal);

    // Increment game results
    if(gameResult == 't')
      tieVal++;
    else if(gameResult == 'w')
      wonVal++;
    else if(gameResult == 'l')
      lostVal++;
    else
      cout << "Error" << endl;
    
    // Message promt to play again
    cout << endl;
    cout << "Do you want to play again? (Y/N) ";
    cin >> continueGame;
  }
  
  // Show the results of the game
  cout << endl;
  cout << "Thanks for playing!" << endl;
  cout << "You won " << wonVal << " game(s), lost " << lostVal << " game(s), and tied " << tieVal << " game(s)." << endl;
  /******** INSERT YOUR CODE ABOVE HERE ********/

  return 0; // signals the operating system that our program ended OK.
}
