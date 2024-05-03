#ifndef CHECKERS_HUMANPLAYER_H
#define CHECKERS_HUMANPLAYER_H
#include "Player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(squareState color);
    void makeMove(CheckersBoard& board) override;
};



#endif //CHECKERS_HUMANPLAYER_H
