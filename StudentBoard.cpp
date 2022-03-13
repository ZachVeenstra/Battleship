#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>

Board::Board(){
    grid = new int[WIDTH * HEIGHT];
    visible = false;
}

Board::Board(const Board& other){
    this->grid = other.grid;
    this->visible = other.visible;
}

/**
 * Copy Operator=
 * Pass object in, steal its data and return itself
 * 
 * @param other 
 * @return Board& 
 */
Board& Board::operator=(const Board& other){
    // std::swap(this->grid, other.grid);
    // std::swap(this->visible, other.visible);

    this->grid = other.grid;
    this->visible = other.visible; // BAD BAD BAD


    return *this;
}

Board::~Board(){
    delete(grid);
}

void Board::setVisible(bool v){
    this->visible = v;
}

int& Board::Internal::operator[](int index){
    if(index >= WIDTH){
        throw std::out_of_range(std::to_string(index) + 
            " is greater value than or equal to grid width of " + 
            std::to_string(WIDTH));
    }
    return _grid[index];
}

Board::Internal Board::operator[](int index){
    if(index >= HEIGHT){
        throw std::out_of_range(std::to_string(index) + 
            " is greater value than or equal to grid height of " + 
            std::to_string(HEIGHT));
    }
    return Board::Internal(grid+(index * WIDTH));
}

std::ostream& operator<<(std::ostream& os, Board const& b){
    if (b.visible) {
        std::cout << " |-------------------- YOUR BOARD --------------------|\n"
                     "        0        1        2        3        4        5        6        7        8        9  \n"
                     "--------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < WIDTH; ++i) {
            std::cout << i << " |     ";
            for (int j = 0; j < HEIGHT; ++j) {
                std::cout << char(b.grid[(i * WIDTH) + j]) << "        ";
                if (char(b.grid[(i * WIDTH) + j]) == EMPTY) {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }

    }
    else {

    }
    return os;
}

int Board::count() const{
    return 0;
}

/**
 * Overloads < to compare boards
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool Board::operator< (const Board& other){
    return false;
}
