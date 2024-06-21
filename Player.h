#ifndef CHECKERS_PLAYER_H
#define CHECKERS_PLAYER_H
#include "CheckersBoard.h"



class Player{
protected:
    Owner color;
    CheckersBoard& board;
public:
    Player(Owner color, CheckersBoard& board) : color(color), board(board) {}
    virtual ~Player() {}
    virtual void makeMove(CheckersBoard& board, int depth, unsigned int random_seed) = 0;




};



#endif //CHECKERS_PLAYER_H
