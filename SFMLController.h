#ifndef CHECKERS_SFMLCONTROLLER_H
#define CHECKERS_SFMLCONTROLLER_H
#include <iostream>
#include "CheckersBoard.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"
#include "SFMLPlayer.h"

enum class PlayerType {
    Human,
    Computer
};

class SFMLController {
private:
    CheckersBoard& board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
public:
    SFMLController(CheckersBoard& board, PlayerType whiteType, PlayerType blackType);
    std::unique_ptr<Player> createPlayer(PlayerType type, Owner color);
    void playGame(sf::Event &event);
};

#endif //CHECKERS_SFMLCONTROLLER_H
