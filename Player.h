#ifndef CHECKERS_PLAYER_H
#define CHECKERS_PLAYER_H
#include "CheckersBoard.h"


class Player{
protected:
    squareState pieceColor;
public:
    Player(squareState color) : pieceColor(color) {}
    virtual ~Player() {}
    virtual void makeMove(CheckersBoard& board) = 0;
};



#endif //CHECKERS_PLAYER_H
