#include "Controller.h"

Controller::Controller(CheckersBoard& board, PlayerType whiteType, PlayerType blackType) : board(board)
{
        whitePlayer = createPlayer(whiteType, squareState::whiteMan);
        blackPlayer = createPlayer(blackType, squareState::blackMan);
}



std::unique_ptr<Player> Controller::createPlayer(PlayerType type, squareState color)
{
    if (type == PlayerType::Human)
    {
        return std::make_unique<HumanPlayer>(color);
    } else
    {
        return std::make_unique<ComputerPlayer>(color);
    }
}

void Controller::playGame() {
    while (!board.isGameOver())
    {
        board.displayBoard();
        blackPlayer->makeMove(board);
        if (board.isGameOver()) {
            std::cout << "Game over!" << std::endl;
            break;  // If the game ends after Black's move, exit the loop
        }

        board.displayBoard();
        whitePlayer->makeMove(board);
        if (board.isGameOver()) {
            std::cout << "Game over!" << std::endl;
            break;
        }
    }
}