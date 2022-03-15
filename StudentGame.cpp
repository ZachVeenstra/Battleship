#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>
#include <ctime>

/**
 * Constructor will create the ships vector and add ships to it.
 */
Game::Game(){
	Ship* carrier = new Ship(5,"Carrier",'C');
	Ship* battlesip = new Ship(4,"Battleship",'B');
	Ship* destroyer = new Ship(3,"Destroyer",'D');
	Ship* submarine = new Ship(3,"Submarine",'S');
	Ship* patrolBoat = new Ship(2,"Patrol Boat",'P');
	ships = {*carrier, *battlesip, *destroyer,
			 *submarine, *patrolBoat};

	Board* p = new Board();
	player = *p;
	player.setVisible(true);

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
   		  "computer players! Place your ships, and good luck to you!\n" << std::endl;
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
	int x; 
	int y;
	int intDirection;
	Direction direction;
	Board* b = new Board();
	b->setVisible(true);


	// prints the different pieces
	std::cout << "The pieces are:\n\n"
   		  "\t" << ships.at(0) << "\n" 
   		  "\t" << ships.at(1) << "\n"
		  "\t" << ships.at(2) << "\n"
          "\t" << ships.at(3) << "\n"
          "\t" << ships.at(4) << std::endl;

	
	for (int i = 0; i < 5; ++i) {
		std::cout << "Where do you wish to place the " << ships.at(i) << 
					 "?" << std::endl;

		std::cin >> x >> y; // accepts coordinates and sets x & y

		std::cout << "Horizontally or Vertically? (0 or 1)" << std::endl;

		std::cin >> intDirection;

		std::cout << "Attempting to place the " << ships.at(i).getName() << 
					 " at " << x << ", " << y;
		if(intDirection == 0) {
			std::cout << " horizontally." << std::endl;
			direction = HORIZONTAL;
		}
		else {
			std::cout << " vertically." << std::endl;
			direction = VERTICAL;
		}

		if (!place(x,y,direction,ships.at(i),*b)) {
			i = i - 1; // Causes the for-loop to enter the current iteration again.
		}
			
		std::cout << *b << std::endl; // prints the board after placement
		
		player = *b;
	}
}

/**
 * Handle the computer placing ships.
 */
void Game::placeShipsPC(){
	// Board* b = new Board();
	// b->setVisible(true);

	// int x = std::clock() % 10;
	// int y = std::clock() % 10;
	// int intDirection = clock() % 2;
	// Direction direction;

	// if(intDirection == 0) {
	// 		direction = HORIZONTAL;
	// }
	// else {
	// 	direction = VERTICAL;
	// }
	
	// for (int i = 0; i < 5; ++i){
	// 	if (!place(x,y,direction,ships.at(i),*b)) {
	// 		std::cout << *b << std::endl;
	// 		std::cout << i << x << y << intDirection << std::endl;
	// 		i = i - 1; // Causes the for-loop to enter the current iteration again.
	// 	}
	// 	//std::cout << *b << std::endl;
	// }

	// computer = *b;
	place(9,1,HORIZONTAL,ships.at(0),computer);
	place(1,2,VERTICAL,ships.at(1),computer);
	place(3,6,HORIZONTAL,ships.at(2),computer);
	place(5,7,VERTICAL,ships.at(3),computer);
	place(4,4,VERTICAL,ships.at(4),computer);
	std::cout << computer << std::endl;
	
}

/**
 * Helper method that checks if it is safe to put a ship
 * at a particular spot with a particular direction.
 */
bool Game::place(const int& x, const int& y, Direction d, const Ship& s, Board& b){
	Board* copycopy = new Board(b);
	Board &copy = *copycopy;
	// this is bad, but it works...
	// Board copy(b);


	bool result = false;

	try {
		for (int i = 0; i < s.getSpaces(); ++i) {
			if (d == HORIZONTAL) {
				if (b[x][y + i] == EMPTY) {
					copy[x][y + i] = s.getChr();
					result = true;
				}
				else {
					std::cout << "That intersects with another ship. Try again.\n";
					return false;
				}
			}
			else {
				if (b[x + i][y] == EMPTY) {
					copy[x + i][y] = s.getChr();
					result = true;
				}
				else {
					std::cout << "That intersects with another ship. Try again.\n";
					return false;
				}
			}
		}
	}
	catch (std::out_of_range) {
		std::cout << "That is out of bounds of the board. Try again.\n";
		return false;
	}

	if(result) {
		b = copy;
	}

	return result;
}

/**
 * Call human turn/computer turn until someone wins.
 */
void Game::run(){
	const int totalSpaces = 17;
	while (player.count() != totalSpaces && 
		   computer.count() != totalSpaces) {
		humanTurn();
		computerTurn();
		std::cout << player << std::endl;
		std::cout << computer << std::endl;
		if (computer.count() < player.count()) {
			std::cout << "HOPPER IS WINNING" << std::endl;
		}
		else {
			std::cout << "YOU ARE WINNING" << std::endl;
		}
		std::cout << "Score: " << computer.count() << " to " << 
					 player.count() << "." << std::endl;
	}
	if (computer.count() < player.count()) {
		std::cout << "HOPPER WON!\nYOU LOSE, LOSER."<< std::endl;
	}
	else {
		std::cout << "YOU WIN!\n" << std::endl;
	}
}

void Game::humanTurn(){
	int x;
	int y;
	std::cout << "Which coordinate do you wish to attack?" << std::endl;
	std::cin >> x >> y;

	char space = computer[x][y];

	if (space != EMPTY &&
	   (space != MISS && space != HIT)) {
		std::cout << "Player HIT!" << std::endl;
		computer[x][y] = HIT;
	}
	else if (space == EMPTY) {
		std::cout << "Player MISS!" << std::endl;
		computer[x][y] = MISS;
	}
	else {
		std::cout << "This space has already been attacked. Try again." << 
					 std::endl;
		humanTurn();
	}
	
	
}

void Game::computerTurn(){
	int x = std::clock() % 10;
	int y = std::clock() % 10;

	char space = player[x][y];

	if (space != EMPTY &&
	   (space != MISS && space != HIT)) {
		std::cout << "Computer HIT!" << std::endl;
		player[x][y] = HIT;
	}
	else if (space == EMPTY) {
		std::cout << "Computer MISS!" << std::endl;
		player[x][y] = MISS;
	}
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
	Game g;
	g.beginGame();

	return 0;
}
