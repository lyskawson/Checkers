#include "HumanPlayer.h"
#include "Player.h"


void HumanPlayer::makeMove(CheckersBoard& board, int depth, unsigned random_seed)
{
    std::vector<std::string> errorCodes = {"Move from invalid field", "Move to invalid field", "No player's piece on 'from' field", "Move to field is not empty", "Wrong geometry of move", "Wrong geometry of move"};
    int from, to;
    bool validMoveMade = false;

    while (!validMoveMade)
    {
        board.displayBoard();

        std::cout << "Enter your move (format: from to, where positions range from 1 to 32): " << std::endl;

        if (!(std::cin >> from >> to)) {
            std::cerr << "Invalid input. Please enter numbers.\n";
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }



        if (from < 1 || from > 32 || to < 1 || to > 32) //change here form 0 to 1
        {
            std::cerr << "Positions must be between 1 and 32.\n";
            continue;
        }

        bool captureRequired = false;
        for (int i = 1; i <= 32; ++i) {
            int piece = board.getPieceAt(i);
            if (board.isPiece(piece) && board.getOwner(piece) == color) {
                if (board.canCapture(i)) {
                    captureRequired = true;
                    break;
                }
            }
        }

        int CAPTURE = (abs(board.getVer(from) - board.getVer(to)) == 2 && abs(board.getHor(from) - board.getHor(to)) == 2) ? 1 : 0;

        if (captureRequired && CAPTURE == 0) {
            std::cerr << "A capture move is mandatory. You must capture.\n";
            continue;
        }


        int moveResult = board.makeMove(color, from, to, CAPTURE);
        if (moveResult != 0 && moveResult != 100)
        {
            std::cerr << "Illegal move. "<< errorCodes[moveResult-1] << ". Please try again.\n";
            continue;
        }
        board.displayBoard();
        bool wasPromoted = (moveResult == 100);

        // Check for additional captures
        if (CAPTURE == 1 && board.canCapture(to) && !wasPromoted)
        {
            std::cout << "You must continue capturing:\n";
            board.displayBoard();
            from = to;
            continue;
        }

        validMoveMade = true;
    }
}





