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
    }
    else
    {
        return std::make_unique<ComputerPlayer>(color, board);
    }
}



void Controller::playGame()
{
    int drawCounter = 0;
    int moveNum = 0;
    const int drawLimit  = 40;
    while (true)
    {
        ++moveNum;
        ++drawCounter;
        if (drawCounter > drawLimit)
        {
            std::cerr << " Exceeded limit of moves by kings without progress - draw" << std::endl;
            std::cout << "DRAW" << std::endl;
            std::exit(40);  // exceeded limit of moves by kings without progress - draw
        }
        int player = moveNum%2;


        if (player == 0)
        {
            whitePlayer->makeMove(board);
        }
        else
        {
            blackPlayer->makeMove(board);
        }

        if (board.isGameOver(1-player))
        {
            std::cout << "Winner: " << (player == 0 ? "White" : "Black") << std::endl;
            std::exit(0);  // end the game
        }

        if (board.isManMoved())
        {
            drawCounter = 0;
        }

    }

}


