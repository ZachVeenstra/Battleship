#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>
#include <ctime>

/**
 * Creates the ships vector, player, and computer boards.
 */
Game::Game(){

	// These are the 5 ships that can be placed on the board.
	Ship* carrier = new Ship(5,"Carrier",'C');
	Ship* battlesip = new Ship(4,"Battleship",'B');
	Ship* destroyer = new Ship(3,"Destroyer",'D');
	Ship* submarine = new Ship(3,"Submarine",'S');
	Ship* patrolBoat = new Ship(2,"Patrol Boat",'P');

	// We add them to the ships vector so they can be accessed later.
	ships = {*carrier, *battlesip, *destroyer,
			 *submarine, *patrolBoat};

	// Create a new board and assign it to player.
	Board* p = new Board();
	player = *p;
	player.setVisible(true);

	// Create a new board and assign it to computer.
	Board* c = new Board();
	computer = *c;
	computer.setVisible(false);
}

/**
* Greets the player and explains the pieces.
*/
void Game::hello(){

	// displays the welcome message
	std::cout << "|--------------- BATTLESHIP ---------------|\n\n"
   		  "You will play versus Admiral Hopper, the greatest of\n"
   		  "computer players! Place your ships, and good luck to you!\n" << 
			 std::endl;

	// prints the different pieces
	std::cout << "The pieces are:\n\n"
   		  "\t" << ships.at(0) << "\n" 
   		  "\t" << ships.at(1) << "\n"
		  "\t" << ships.at(2) << "\n"
          "\t" << ships.at(3) << "\n"
          "\t" << ships.at(4) << std::endl;
}


/**
 * Begin Game lets user and then computer setup boards then calls run()
 */
void Game::beginGame(){

	// greets the player
	hello();

	// player sets up board
	placeShips();

	// computer sets up board
	placeShipsPC();

	// runs the game
	run();
}

/**
 * Handle the human placing ships.
 */
void Game::placeShips(){
	// Coordinates for ship placement.
	int x; 
	int y;
	
	// The number of ships that can be placed.
	const int numShips = 5;

	// The direction of the ship. 0 for horizontal, 1 for vertical.
	int intDirection;

	// The direction to be given to place().
	Direction direction;

	// Loop through the ships
	for (int i = 0; i < numShips; ++i) {
								      				// This prints the ship being placed
		std::cout << "Where do you wish to place the " << ships.at(i) << 
					 "?" << std::endl;

		std::cin >> x >> y; // accepts coordinates and sets x & y

		std::cout << "Horizontally or Vertically? (0 or 1)" << std::endl;

		std::cin >> intDirection; // Accepts an integer for the direction

		std::cout << "Attempting to place the " << ships.at(i).getName() << 
					 " at " << x << ", " << y;

		// If the desired direction is horizontal.
		if(intDirection == 0) {
			std::cout << " horizontally." << std::endl;
			direction = HORIZONTAL;
		}
		// If the desired direction is vertical.
		else if (intDirection == 1) {
			std::cout << " vertically." << std::endl;
			direction = VERTICAL;
		}
		// If there was an incorrect entry.
		else {
			std::cout << "\n\n\n\n\nIncorrect entry, try again." <<
						 std::endl;
		}

		// Check if the direction entry was acceptable and attempt a
		// ship placement.
		if (intDirection <= 1 && !place(x,y,direction,ships.at(i),player)) {
			// Causes the for-loop to enter the current iteration again.
			i = i - 1; 
		}
			
		std::cout << player << std::endl; // prints the board after placement
		
	}
}

/**
 * Handle the computer placing ships. It picks random places based on
 * the clock time of the machine.
 */
void Game::placeShipsPC(){
	// Coordinates for ship placement.
	int x; 
	int y;
	
	// The number of ships that can be placed.
	const int numShips = 5;

	// The direction of the ship. 0 for horizontal, 1 for vertical.
	int intDirection;

	// The direction to be given to place().
	Direction direction;

	std::cout << "\n\n\nThis is the output of HOPPER placing her ships."
				 "\n-----------------------------------------------" <<
				 std::endl;

	// Loop through all the ships.
	for (int i = 0; i < numShips; ++i){
		
		/* This takes the last digit of the machines current clock time.
		This gives a random number for the coordinates. */
		x = clock() % 10;
		y = clock() % 10;

		// This will use that same clock time and get either a 0 or a 1.
		int intDirection = clock() % 2;

		// If it is 0, the direction will be horizontal.
		if(intDirection == 0) {
			direction = HORIZONTAL;
		}
		// If it's 1, the direction will be vertical.
		else {
			direction = VERTICAL;
		}
		// Attempt to place the ship.
		if (!place(x,y,direction,ships.at(i),computer)) {
			i = i - 1; // Causes the for-loop to enter the current iteration again.
		}
	}
	std::cout << "\n-----------------------------------------------" <<
				 std::endl;
}

/**
 * Helper method that checks if it is safe to put a ship
 * at a particular spot with a particular direction. A ship must not 
 * collide with another ship, and must be fully on the board.
 */
bool Game::place(const int& x, const int& y, Direction d, const Ship& s, Board& b){

	/* Use the copy constructor to create a copy of b. We need this just 
	in case the placement goes wrong. */
	Board copy(b);

	bool result = false;

	try {
		// Loop through the amount of spaces the ship takes up
		for (int i = 0; i < s.getSpaces(); ++i) {
			// If we want to place horizontally.
			if (d == HORIZONTAL) {
				// If the space is empty:
				if (copy[x][y + i] == EMPTY) {
					// Place the character representation of the ship.
					copy[x][y + i] = s.getChr();
					result = true;
				}
				// If the space is occupied.
				else {
					std::cout << "That intersects with another ship. Try again.\n";
					return false;
				}
			}
			// If we want to place the ship vertically.
			else {
				// If the space is empty
				if (copy[x + i][y] == EMPTY) {
					// Place the character representation of the ship.
					copy[x + i][y] = s.getChr();
					result = true;
				}
				// If the space is occupied.
				else {
					std::cout << "That intersects with another ship. Try again.\n";
					return false;
				}
			}
		}
	}
	// If the space is outside of the board, we will throw an exception.
	catch (std::out_of_range) {
		std::cout << "That is out of bounds of the board. Try again.\n";
		return false;
	}

	// If all the spaces we place the ship in were valid.
	if(result) {
		// Set the original board equal to the copy we made of it.
		b = copy;
	}

	// True upon success.
	return result;
}


/**
 * Calls human turn/computer turn until someone wins. It also displays
 * who is currently winning
 */
void Game::run(){

	// The total amount of spaces the ships take up on a board.
	const int totalSpaces = 17;

	// While both boards have still have ships, keep the game going.
	while (player.count() != totalSpaces && 
		   computer.count() != totalSpaces) {
		
		// The human attacks.
		humanTurn();

		// The computer attacks.
		computerTurn();

		// Print the boards.
		std::cout << player << std::endl;
		std::cout << computer << std::endl;

		// Print who is winning.
		if (computer < player) {
			std::cout << "HOPPER IS WINNING" << std::endl;
		}
		else {
			std::cout << "YOU ARE WINNING" << std::endl;
		}

		// Print the score.
		std::cout << "Score: " << computer.count() << " to " << 
					 player.count() << "." << std::endl;
	}

	// If the computer has less hits on their board, they win.
	if (computer < player) {
		std::cout << "HOPPER WON!\nYOU LOSE, LOSER."<< std::endl;
	}
	// Otherwise the human wins.
	else {
		std::cout << "YOU WIN!\n" << std::endl;
	}
}


/**
 * The human picks which coordinate to attack.
 * 
 */
void Game::humanTurn(){

	// The coordinates on the board
	int x;
	int y;

	std::cout << "Which coordinate do you wish to attack?" << std::endl;
	// The human picks which x and y coordinates to attack.
	std::cin >> x >> y;

	// The value that occupies the attacked space.
	char space = computer[x][y];

	// If the value is a ship:
	if (space != EMPTY &&
	   space != MISS && space != HIT) {
		std::cout << "Player HIT!" << std::endl;
		// Change the value to hit.
		computer[x][y] = HIT;
	}
	// If the value is empty:
	else if (space == EMPTY) {
		std::cout << "Player MISS!" << std::endl;
		// Chanve the value to miss.
		computer[x][y] = MISS;
	}
	// If the value is hit or miss, it has already been attacked.
	else {
		std::cout << "That space has already been attacked."
					 " Try again, stupid human." << 
					 std::endl;
		// Restart the stupid human's turn.
		humanTurn();
	}
	
	
}


/**
 * The computer randomly picks which coordinate to attack.
 * 
 */
void Game::computerTurn(){

	/* This takes the last digit of the machines current clock time.
		This gives a random number for the coordinates. */
	int x = std::clock() % 10;
	int y = std::clock() % 10;

	// The value at the attacked space.
	char space = player[x][y];

	// If the value is a ship.
	if (space != EMPTY &&
	   space != MISS && space != HIT) {
		std::cout << "Computer HIT!" << std::endl;
		// Change the value to hit.
		player[x][y] = HIT;
	}
	// If the value is empty.
	else if (space == EMPTY) {
		std::cout << "Computer MISS!" << std::endl;
		// Change the value to miss.
		player[x][y] = MISS;
	}
	// HOPPER tried to attack a space she already had, makes sense given
	// that she is 115 years old...
	else {
		computerTurn(); // the space was already attacked, retry
	}


}

/**
 * Create a game instance and start.
 */
int main(int argc, char** argv){
	(void)argc;
	(void)argv;
	// The game instance.
	Game g;
	// Let the games begin!
	g.beginGame();

	return 0;
}
