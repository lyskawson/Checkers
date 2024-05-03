#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(squareState color) : Player(color)
{

}

void HumanPlayer::makeMove(CheckersBoard& board)
{
    int x1, y1, x2, y2;
    bool validMoveMade = false;

    while (!validMoveMade)
    {
        std::cout << "Enter your move (format: x1 y1 x2 y2): ";
        if (!(std::cin >> x1 >> y1 >> x2 >> y2))
        {
            std::cout << "Invalid input. Please enter numbers.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (board.hasMandatoryCapture(x1, y1) && !board.isCaptureMove(x1, y1, x2, y2))
        {
            std::cout << "A capture move is mandatory. You must capture.\n";
            continue;
        }


        if (!board.isLegalMove(x1, y1, x2, y2))
        {
            std::cout << "Illegal move. Please try again.\n";
            continue;
        }


        board.executeMove(x1, y1, x2, y2);  // This function needs to handle capturing and king promotion


        if (board.canContinueCapture(x2, y2))
        {
            std::cout << "You must continue capturing:\n";
            continue;  // The player must keep moving the same piece if more captures are possible
        }

        validMoveMade = true;
    }

}

bool HumanPlayer::isHuman() const
{
    return true;
}