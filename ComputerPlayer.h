#ifndef CHECKERS_COMPUTERPLAYER_H
#define CHECKERS_COMPUTERPLAYER_H
#include "Player.h"

class ComputerPlayer : public Player {
public:
    ComputerPlayer(squareState color);
    void makeMove(CheckersBoard& board) override;

};

#endif //CHECKERS_COMPUTERPLAYER_H
