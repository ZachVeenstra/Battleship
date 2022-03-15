#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>


/**
 * Construct a new Board object.
 * 
 */
Board::Board(){
    // The grid is a square as tall as HEIGHT and as wide as WIDTH.
    grid = new int[WIDTH * HEIGHT];

    // The board is initially invisible.
    visible = false;
}


/**
 * Copy constructor. Makes a deep copy of the contents of other and 
 * assigns them to this.
 * 
 * @param other The board whose contents will be copied.
 */
Board::Board(const Board& other){

    // A new grid is created for this.
    this->grid = new int[WIDTH * HEIGHT];

    // Loop through and copy the contents of other's grid into this.
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        this->grid[i] = other.grid[i];
    }

    // Copy the visibility of other into this.
    this->visible = other.visible;
}


/**
 * Copy Operator=: 
 * Overloads the assignment operator so that the contents of other will
 * be deeply copied into an existing board.
 * 
 * @param other The board whose contents will be copied.
 * @return Board& - The existing board that inherits the contents of 
 *                  other.
 */
Board& Board::operator=(const Board& other){
    /* This uses the copy-and-swap implementation. Other is an existing
    board that this board is being set equal to. We want all the
    elements of this to be equal to other without occupying the same 
    memory. In order to do this, we first create a copy of other, then
    swap the values of the variables into this board, destruct the copy,
    and then return this. This will make sure this boards' variables
    have the same values as other. */

    // Create a deep copy of other using the copy constructor.
    Board* copy = new Board(other);

    // Swap the variables of copy into this.
    std::swap(this->visible, copy->visible);
    std::swap(this->grid, copy->grid);

    // Destruct copy.
    copy->~Board();

    return *this;
}


/**
 * Destroys the Board object.
 * 
 */
Board::~Board(){
    delete(grid);
}


/**
 * Sets whether the board is visible or not.
 * 
 * @param v True for visible, false for invisible.
 */
void Board::setVisible(bool v){
    this->visible = v;
}


/**
 * Overloads the [] operator so you can select a column number in grid.
 * 
 * @param index The column number.
 * @return int& 
 */
int& Board::Internal::operator[](int index){

    // If the index goes outside the board.
    if(index >= WIDTH){
        // Throw an exception.
        throw std::out_of_range(std::to_string(index) + 
            " is greater value than or equal to grid width of " + 
            std::to_string(WIDTH));
    }
    return _grid[index];
}

/**
 * Overloads the [] operator so you can selecta a row number in grid.
 * 
 * @param index The row number.
 * @return Board::Internal 
 */
Board::Internal Board::operator[](int index){

    // If the index goes outside the board.
    if(index >= HEIGHT){
        // Throw an exception
        throw std::out_of_range(std::to_string(index) + 
            " is greater value than or equal to grid height of " + 
            std::to_string(HEIGHT));
    }
    return Board::Internal(grid+(index * WIDTH));
}


/**
 * Overloads the << operator so the board can be put on the stream with 
 * a visual representation.
 * 
 * @param os The outstream.
 * @param b The board being streamed.
 * @return os The outstream.
 */
std::ostream& operator<<(std::ostream& os, Board const& b){

    // The current value of the space.
    char space;

    // If the board is visible, we want to see all the ships.
    if (b.visible) {
        os << " |-------------------- YOUR BOARD --------------------|\n"
                     "        0        1        2        3        4        5        6        7        8        9  \n"
                     "--------------------------------------------------------------------------------------------\n";
        
        // loop through the number of rows
        for (int i = 0; i < HEIGHT; ++i) {
            os << i << " |     ";
            
            // loop through the number of columns
            for (int j = 0; j < WIDTH; ++j) {
                // i * width determines which row, j determines the column
                space = char(b.grid[(i * WIDTH) + j]); 
                os << space << "        ";
                // if the space is empty, we print a space.
                if (space == EMPTY) {
                    os << " ";
                }
            }
            // End with a new line.
            os << "\n";
        }

    }

    // If the board is invisible, we only show hits and misses.
    else {
        os << " |-------------------- COMPUTER BOARD --------------------|\n"
                     "        0        1        2        3        4        5        6        7        8        9  \n"
                     "--------------------------------------------------------------------------------------------\n";
        
        // loop through rows
        for (int i = 0; i < HEIGHT; ++i) {
            os << i << " |     ";

            // loop through columns
            for (int j = 0; j < WIDTH; ++j) {
                // i * width determines which row, j determines the column
                space = char(b.grid[(i * WIDTH) + j]);
                // If the space is a ship we don't print it.
                if (space != HIT &&
                    space != MISS) { 
                    os << "         ";
                }
                // We do print hits and misses.
                if (space == HIT ||
                    space == MISS) {
                    os << space << "        ";
                }
            }
            // End with a new line.
            os << "\n";
        }
    }
    // Give back the outstream.
    return os;
}


/**
 * Counts the number of hits on the board.
 * 
 * @return Count - The number of hits.
 */
int Board::count() const{

    // Start hits out at zero.
    int hits = 0;

    // Loop through the board.
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        // If we find a hit:
        if (grid[i] == HIT) {
            // Increment hits.
            hits++;
        }
    }
    return hits;
}

/**
 * Overloads < to compare boards. A board is less than another if it has
 * less hits on it.
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool Board::operator< (const Board& other){

    bool result;

    // if there are less hits on this than other, it is less than it.
    if (this->count() < other.count()) {
        result = true;
    }
    // If not, other is less than this.
    else {
        result = false;
    }

    return result;
}
