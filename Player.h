#ifndef CHECKERS_PLAYER_H
#define CHECKERS_PLAYER_H
#include "CheckersBoard.h"
#include "SFML/Graphics.hpp"


class Player{
protected:
    Owner color;
    CheckersBoard& board;
public:
    Player(Owner color, CheckersBoard& board) : color(color), board(board) {}
    virtual ~Player() {}
    virtual void makeMove(CheckersBoard& board) = 0;
    virtual void makeMove(CheckersBoard& board, sf::Event &event) {} // new fun


};



#endif //CHECKERS_PLAYER_H
