#include "SFMLPlayer.h"


void SFMLPlayer::makeMove(CheckersBoard& board, sf::Event& event) {
    static int from = -1;
    static bool selectingFrom = true;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int row = (event.mouseButton.y - 57) / 117;
        int col = (event.mouseButton.x-57) / 117;

        // Ensure row and col are within valid range
        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            return;
        }

        int field = row * 4 + col / 2 + 1;

        if (selectingFrom) {
            if (board.getPieceAt(field) != EMPTY_F && board.getOwner(board.getPieceAt(field)) == color) {
                from = field;
                selectingFrom = false;
            }
        } else {
            int to = field;
            int CAPTURE = (abs(board.getVer(from) - board.getVer(to)) == 2 && abs(board.getHor(from) - board.getHor(to)) == 2) ? 1 : 0;

            int moveResult = board.makeMove(color, from, to, CAPTURE);
            if (moveResult == 0) {
                selectingFrom = true;

                // Check for additional captures
                if (CAPTURE == 1 && board.canCapture(to)) {
                    from = to;
                    selectingFrom = false;
                }
            } else {
                std::cout << "Illegal move. Error code: " << moveResult << ". Please try again.\n";
                selectingFrom = true;
            }
        }
    }
}


