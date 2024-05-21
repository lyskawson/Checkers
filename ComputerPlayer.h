#ifndef CHECKERS_COMPUTERPLAYER_H
#define CHECKERS_COMPUTERPLAYER_H
#include "Player.h"

class ComputerPlayer : public Player
        {
public:
    ComputerPlayer(Owner color, CheckersBoard& board) : Player(color, board) {}
    void makeMove(CheckersBoard& board) override;
    bool isHuman() const;

};

#endif //CHECKERS_COMPUTERPLAYER_H
