#include "CheckersBoard.h"

CheckersBoard::CheckersBoard()
{
    board.resize(8, 8);
    restartBoard();
}

void CheckersBoard::restartBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((i + j) % 2 != 0)
            {
                if (i < 3) {
                    board[i][j].state = squareState::blackMan;
                } else if (i > 4) {
                    board[i][j].state = squareState::whiteMan;
                } else {
                    board[i][j].state = squareState::empty;
                }
            } else {
                board[i][j].state = squareState::empty;
            }
        }
    }
}

void CheckersBoard::displayBoard() const
{
    for (int i = 0; i < 8; ++i)
    {
        const Array<Field> &row = board[i];
        for (int j = 0; j < 8; ++j)
        {
            const Field& field = row[j];
            char c = ' ';
            if ((i + j) % 2 != 0)
            {
                c = '.';  // Change default character for black squares
            }

            switch (field.state)
            {
                case squareState::whiteMan: c = 'p'; break;
                case squareState::blackMan: c = 'q'; break;
                case squareState::whiteKing: c = 'P'; break;
                case squareState::blackKing: c = 'Q'; break;
                default: break;  // Empty squares already have their default characters set above
            }
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool CheckersBoard::isGameOver() const
{
    bool hasWhitePieces = false;
    bool hasBlackPieces = false;
    bool whiteCanMove = false;
    bool blackCanMove = false;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            const Field &field = board[i][j];
            switch (field.state)
            {
                case squareState::whiteMan:
                case squareState::whiteKing:
                    hasWhitePieces = true;
                    if (canMove(i, j))
                        whiteCanMove = true;
                    break;
                case squareState::blackMan:
                case squareState::blackKing:
                    hasBlackPieces = true;
                    if (canMove(i, j))
                        blackCanMove = true;
                    break;
                default:
                    break;
            }
            if (hasWhitePieces && hasBlackPieces && whiteCanMove && blackCanMove)
                return false;
        }
    }
}

bool CheckersBoard::canMove(int row, int col) const
{
    return checkForNormalMoves(row, col) || checkForCaptureMoves(row, col);
}

bool CheckersBoard::checkForNormalMoves(int row, int col) const
{
    const Field& field = board[row][col];
    if (field.state == squareState::empty)
        return false;

    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int limit = (field.state == squareState::whiteKing || field.state == squareState::blackKing) ? 8 : 1;

    for (int d = 0; d < 4; ++d)
    {
        for (int step = 1; step <= limit; ++step)
        {
            int new_row = row + directions[d][0] * step;
            int new_col = col + directions[d][1] * step;
            if (new_row < 0 || new_row >= 8 || new_col < 0 || new_col >= 8)
                break;
            if (board[new_row][new_col].state != squareState::empty)
                break;
            if (step == 1 || field.state == squareState::whiteKing || field.state == squareState::blackKing)
                return true;
        }
    }

    return false;
}

bool CheckersBoard::checkForCaptureMoves(int row, int col) const
{
    const Field& field = board[row][col];
    if (field.state == squareState::empty) return false;

    squareState enemy[2] = {(field.state == squareState::whiteMan || field.state == squareState::whiteKing) ? squareState::blackMan : squareState::whiteMan,
                    (field.state == squareState::whiteMan || field.state == squareState::whiteKing) ? squareState::blackKing : squareState::whiteKing};
    int directions[4][2] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}}; // Directions for potential jumps

    for (int d = 0; d < 4; ++d) {
        int mid_row = row + directions[d][0] / 2;
        int mid_col = col + directions[d][1] / 2;
        int new_row = row + directions[d][0];
        int new_col = col + directions[d][1];

        if (new_row < 0 || new_row >= 8 || new_col < 0 || new_col >= 8)
            continue;
        if (board[new_row][new_col].state != squareState::empty)
            continue;

        if (board[mid_row][mid_col].state == enemy[0] || board[mid_row][mid_col].state == enemy[1])
            return true;
    }

    return false;
}