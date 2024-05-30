#include "Player.h"
#include <SFML/Graphics.hpp>

#ifndef CHECKERS_SFMLPLAYER_H
#define CHECKERS_SFMLPLAYER_H

class SFMLPlayer : public Player
{
public:
    SFMLPlayer(Owner color, CheckersBoard& board) : Player(color, board) {}
    void makeMove(CheckersBoard& board, sf::Event &event) override;
    void makeMove(CheckersBoard& board) override {};

};

#endif //CHECKERS_SFMLPLAYER_H
