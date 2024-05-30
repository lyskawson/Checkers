#ifndef CHECKERS_HUMANPLAYER_H
#define CHECKERS_HUMANPLAYER_H
#include "Player.h"
#include <SFML/Graphics.hpp>


class HumanPlayer : public Player
        {
public:
    HumanPlayer(Owner color, CheckersBoard& board) : Player(color, board) {}
    void makeMove(CheckersBoard& board) override;


};



#endif //CHECKERS_HUMANPLAYER_H
