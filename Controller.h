#ifndef CHECKERS_CONTROLLER_H
#define CHECKERS_CONTROLLER_H
#include <iostream>
#include "CheckersBoard.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

enum class PlayerType {
    Human,
    Computer
};

class Controller {
private:
    CheckersBoard& board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
public:
    Controller(CheckersBoard& board, PlayerType whiteType, PlayerType blackType);
    std::unique_ptr<Player> createPlayer(PlayerType type, Owner color);


    void playGame();
};
#endif //CHECKERS_CONTROLLER_H
