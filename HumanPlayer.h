#ifndef CHECKERS_HUMANPLAYER_H
#define CHECKERS_HUMANPLAYER_H
#include "Player.h"



class HumanPlayer : public Player
        {
public:
    HumanPlayer(Owner color, CheckersBoard& board) : Player(color, board) {}
    void makeMove(CheckersBoard& board, int depth=0, unsigned int random_seed = 0) override;


};



#endif //CHECKERS_HUMANPLAYER_H
