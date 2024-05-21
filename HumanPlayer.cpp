#include "HumanPlayer.h"



void HumanPlayer::makeMove(CheckersBoard& board) {
    int from, to;
    bool validMoveMade = false;

    while (!validMoveMade) {
        std::cout << "Enter your move (format: from to, where positions range from 1 to 32): ";
        if (!(std::cin >> from >> to)) {
            std::cout << "Invalid input. Please enter numbers.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }



        if (from < 0 || from >= 32 || to < 0 || to >= 32)
        {
            std::cout << "Positions must be between 1 and 32.\n";
            continue;
        }

        int CAPTURE = (abs(board.getVer(from) - board.getVer(to)) == 2 && abs(board.getHor(from) - board.getHor(to)) == 2) ? 1 : 0;

        int moveResult = board.makeMove(color, from, to, CAPTURE);
        if (moveResult != 0)
        {
            std::cout << "Illegal move. Error code: " << moveResult << ". Please try again.\n";
            continue;
        }

        // Check for additional captures
        if (CAPTURE == 1 && board.canCapture(to))
        {
            std::cout << "You must continue capturing:\n";
            continue;
        }

        validMoveMade = true;
    }
}

bool HumanPlayer::isHuman() const
{
    return true;
}