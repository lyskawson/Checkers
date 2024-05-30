#ifndef CHECKERS_COMPUTERPLAYER_H
#define CHECKERS_COMPUTERPLAYER_H
#include "Player.h"

class ComputerPlayer : public Player
        {
public:
    ComputerPlayer(Owner color, CheckersBoard& board) : Player(color, board) {}
    void makeMove(CheckersBoard& board) override;
    void makeMove1(CheckersBoard& board);
    bool isHuman() const;
private:
    int alphaBeta(CheckersBoard& board, int depth, int alpha, int beta, bool maximizingPlayer);
    int evaluateBoard(const CheckersBoard& board);

};

#endif //CHECKERS_COMPUTERPLAYER_H
