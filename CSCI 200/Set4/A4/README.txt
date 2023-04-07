Player Class:
	+ Player()
	+ Player(const in PLAYER_NUM)
	+ getPlayer() : int
	+ getChipCount() : int
	+ chipToPlayer(Player& Player) : void
	+ chipToCenter() : void
	- playerNum : int
	- numChips : int

I chose to strcuture my Player class with these functions and variables, I only had two private variables one for the player name/# and the other is to store the amount of chips the player had. I have a defualt constructor that sets the player number and chips to 1 and 3 as the respective defaults however if a intger is passed as a paramter then it will set the player number to whatever the value that was passed. This will help set the player names if their are more than one player being created. I chose to split my player class into two seperate files a ".h" and ".cpp" for function implementation because when I had it one ".hpp" I kept getting errors thrown and I do not know what was causing it so I decided to make them two seperate for the fucntion prototype and definition. In the player class it would be able to give a chip to another player through a reference to that Player, increase their chips by 1 and descrease this chip's by 1. The chips to center would only decrease the players chip by one. 

Game:

When creating the game functionality I decided to not make it into a class and keept it seperated by function prototypes and function implementations. I seperated the protopyes into a ".h" with the implemention in a ".cpp". My game file included only 7 functions with zero global variables. The functions were a rollDice, passToLeft/Right, reverse the direction the game is being played, move to the next player, and finally play a turn. In the ".cpp" it would have all the implementations of those function protoypes. The passLeft and passRight had pretty similar funcationality however instead of going to the next Node in the list for the passRight it woudl go to previous Node in the list for the passLeft. The movePlayer would just set the current Node either to next or the previous depending on direction. The playTurn would take in the numbers that dice has rolled and perfom the game functionality that needed to be perfomed including reversing the direction, skipping the player, and giving chips to either the left or the right. The end of the function would return a boolean value depending wether the player had zero chips or the had more than zero chips.

I had a Player Class, Node Class, and LinkedList Class, in terms of functions I had game functionality that was only comprised of functions and was not based off a class. I decided to implement it this way because their were going to multiple players and that needed to be a class so I could make a LinkedList of Players with multiple player objects within that list. I decided to opt out of a class for the game functionality becuase their didnt need to be any game objects for the program to work, it just needed to be game functions that would make use of the Player class.

Extra Credit:

# Players | # Turns
----------|--------
     2    |	 9.8
     3    |  26.2
     4    |  39.4
     5    |  65.5
    10    |  203.8
    20    |  483.9

1. The average number of games that I needed to play to acheive a pretty consistent result with a small margin of error was about 30 games.
2. The relationship between the amount of players and the number of turns is a linear growth.