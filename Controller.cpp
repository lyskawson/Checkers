#include "Controller.h"
#include "CheckersBoard.h"

Controller::Controller(CheckersBoard& board, PlayerType whiteType, PlayerType blackType) : board(board)
{
        whitePlayer = createPlayer(whiteType, Owner::WHITE);
        blackPlayer = createPlayer(blackType, Owner::BLACK);
}



std::unique_ptr<Player> Controller::createPlayer(PlayerType type, Owner color)
{
    if (type == PlayerType::Human)
    {
        return std::make_unique<HumanPlayer>(color, board);
    } else
    {
        return std::make_unique<ComputerPlayer>(color, board);
    }
}



void Controller::playGame()
{
    while (1)
    {
        board.displayBoard();
        blackPlayer->makeMove(board);
        board.displayBoard();
        whitePlayer->makeMove(board);
    }

}