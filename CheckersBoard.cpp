#include "CheckersBoard.h"
#include <iomanip>


CheckersBoard::CheckersBoard()
{

    restartBoard();
}

void CheckersBoard::restartBoard()
{
    /*BLACK_M = 51,
    WHITE_M = 52,
    BLACK_K = 53,
    WHITE_K = 54,
    EMPTY_F = 55,
    INVALID = 56,
    OUTSIDE = 57
    valid fields are numerated from 1 to 32 and invalid fields are numerated from -2 to -1*/

    board = { { 57, 57, 57, 57, 57, 57, 57, 57, 57, 57 },
              { 57, 56, 51, 56, 51, 56, 51, 56, 51, 57 },
              { 57, 51, 56, 51, 56, 51, 56, 51, 56, 57 },
              { 57, 56, 51, 56, 51, 56, 51, 56, 51, 57 },
              { 57, 55, 56, 55, 56, 55, 56, 55, 56, 57 },
              { 57, 56, 55, 56, 55, 56, 55, 56, 55, 57 },
              { 57, 52, 56, 52, 56, 52, 56, 52, 56, 57 },
              { 57, 56, 52, 56, 52, 56, 52, 56, 52, 57 },
              { 57, 52, 56, 52, 56, 52, 56, 52, 56, 57 },
              { 57, 57, 57, 57, 57, 57, 57, 57, 57, 57 } };

    fields = {{ -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 },
              { -2, -1,  1, -1,  2, -1,  3, -1,  4, -2 },
              { -2,  5, -1,  6, -1,  7, -1,  8, -1, -2 },
              { -2, -1,  9, -1, 10, -1, 11, -1, 12, -2 },
              { -2, 13, -1, 14, -1, 15, -1, 16, -1, -2 },
              { -2, -1, 17, -1, 18, -1, 19, -1, 20, -2 },
              { -2, 21, -1, 22, -1, 23, -1, 24, -1, -2 },
              { -2, -1, 25, -1, 26, -1, 27, -1, 28, -2 },
              { -2, 29, -1, 30, -1, 31, -1, 32, -1, -2 },
              { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 } };

    hor = {-1, 2, 4, 6, 8, 1, 3, 5, 7, 2, 4, 6, 8, 1, 3, 5, 7, 2, 4, 6, 8, 1, 3, 5, 7, 2, 4, 6, 8, 1, 3, 5, 7, -1};
    ver = {-1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, -1};

}

const char* symbols[] = {"!!!", " B ", " W ", "(B)", "(W)", "___", "   ", "***", "???"};

void CheckersBoard::displayBoard() const
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            std::cout << symbols[board[i][j] - 50];
        }
        std::cout << std::endl;
    }
}

bool CheckersBoard::isGameOver(int player) const {
    int i, count = 0, content;
    bool is_stuck = true;
    for (i = 1; i <= 32; ++i)
    {
        content = board[ver[i]][hor[i]];
        if (isPiece(content) && (getOwner(content) == player))
        {
            ++count;
            if (canMove(i) || canCapture(i))
            {
                is_stuck = false;
                break;
            }
        }
    }
    if (count == 0 || is_stuck)
        return true;

    return false;
}



Owner CheckersBoard::getOwner(int piece) const
{
    return static_cast<Owner>((piece - 50) % 2);
}

bool CheckersBoard::isMan(int piece) const
{
    return (piece>=BLACK_M) &&(piece<=WHITE_M);
}

bool CheckersBoard::canMove(int field) const
{
    int vert = ver[field];
    int horz = hor[field];
    int piece = board[vert][horz];


    // check for normal direction, man or king
    if (getOwner(piece)==BLACK) //check move down
    {
        if (board[vert+1][horz-1]==EMPTY_F)
            return 1; //left
        if (board[vert+1][horz+1]==EMPTY_F)
            return 1; //right
    }
    if (getOwner(piece)==WHITE) // check move up
    {
        if (board[vert-1][horz-1]==EMPTY_F)
            return 1; //left
        if (board[vert-1][horz+1]==EMPTY_F)
            return 1; //right
    }
    if (isMan(piece))
        return 0; // no more options for man to move

    // now we know it's a king
    if (getOwner(piece)==WHITE) //check for move down
    {
        if (board[vert+1][horz-1]==EMPTY_F)
            return 1; //left
        if (board[vert+1][horz+1]==EMPTY_F)
            return 1; //right
    }
    if (getOwner(piece)==BLACK) //check for move up
    {

        if (board[vert-1][horz-1]==EMPTY_F)
            return 1; //left
        if (board[vert-1][horz+1]==EMPTY_F)
            return 1; //right
    }
    return 0;
}

bool CheckersBoard::canCapture(int field) const
{
    int vert = ver[field];
    int horz = hor[field];
    int piece = board[vert][horz];
    int vert2, vert3;

    //check captures in normal directions
    if (getOwner(piece)==BLACK) //check for capture black down
    {
        vert2 = vert+1; vert3 = vert+2;
        if ((vert3<=8)&&((horz-2)>=1)) // within board to the left
            if ((board[vert3][horz-2]==EMPTY_F) && isPiece(board[vert2][horz-1]) && (getOwner(board[vert2][horz-1])==WHITE))
                return 1;
        if ((vert3<=8)&&((horz+2)<=8)) //within board to the right
            if ((board[vert3][horz+2]==EMPTY_F) && isPiece(board[vert2][horz+1]) && (getOwner(board[vert2][horz+1])==WHITE))
                return 1;
    }
    if (getOwner(piece)==WHITE)  //check for capture white up
    {
        vert2 = vert-1; vert3 = vert-2;
        if ((vert3>=1)&&((horz-2)>=1)) //within board to the left
            if ((board[vert3][horz-2]==EMPTY_F) && isPiece(board[vert2][horz-1]) && (getOwner(board[vert2][horz-1])==BLACK))
                return 1;
        if ((vert3>=1)&&((horz+2)<=8)) //within board to the right
            if ((board[vert3][horz+2]==EMPTY_F) &&
                isPiece(board[vert2][horz+1]) &&
                (getOwner(board[vert2][horz+1])==BLACK)) return 1;
    }
    if (isMan(piece))
        return 0; // no other captures for man

    // now we know it's a king
    if (getOwner(piece)==WHITE) // check for capture white  down
    {
        vert2 = vert+1; vert3 = vert+2;
        if ((vert3<=8)&&((horz-2)>=1)) //within board to the left
            if ((board[vert3][horz-2]==EMPTY_F) && isPiece(board[vert2][horz-1]) && (getOwner(board[vert2][horz-1])==BLACK))
                return 1;
        if ((vert3<=8)&&((horz+2)<=8)) //within board to the right
            if ((board[vert3][horz+2]==EMPTY_F) && isPiece(board[vert2][horz+1]) && (getOwner(board[vert2][horz+1])==BLACK))
                return 1;
    }
    if (getOwner(piece)==BLACK) // check for capture black up
    {
        vert2 = vert-1; vert3 = vert-2;
        if ((vert3>=1)&&((horz-2)>=1)) //within board to the left
            if ((board[vert3][horz-2]==EMPTY_F) && isPiece(board[vert2][horz-1]) && (getOwner(board[vert2][horz-1])==WHITE))
                return 1;
        if ((vert3>=1)&&((horz+2)<=8)) //within board to the right
            if ((board[vert3][horz+2]==EMPTY_F) && isPiece(board[vert2][horz+1]) && (getOwner(board[vert2][horz+1])==WHITE))
                return 1;
    }

    return 0;
}

bool CheckersBoard::isPiece(int piece) const
{
    return (piece>=BLACK_M)&&(piece<=WHITE_K) ;
}

bool CheckersBoard::isKing(int piece) const
{
    return (piece>=BLACK_K)&&(piece<=WHITE_K);
}

int CheckersBoard::makeMove(int player, int from, int to, int CAPTURE)
{
    int result;
    int piece_mid; /* tylko przy biciu */
    int piece_from=board[ver[from]][hor[from]];
    int field_to=board[ver[to]][hor[to]];

    if ((from<1) || (from>32))
        return 1; //from invalid feild
    if ((to<1) || (to>32))
        return 2; //field to ivalid
    if ((!isPiece(piece_from)) || (getOwner(piece_from) != player))
        return 3; //no player's piece on 'from' field
    if (field_to != EMPTY_F)
        return 4; //field 'to' is not empty
    if ((CAPTURE==0)&&((abs(ver[from]-ver[to])!=1)||(abs(hor[from]-hor[to])!=1)))
        return 5; //wrong geometry of move
    if ((CAPTURE==1)&&((abs(ver[from]-ver[to])!=2)||(abs(hor[from]-hor[to])!=2)))
        return 6; //wrong geometry of move
    if (CAPTURE==1)
    {
        piece_mid=board[(ver[from]+ver[to])/2][(hor[from]+hor[to])/2];
        if ((!isPiece(piece_mid)) || (getOwner(piece_mid) != getOpponent(player)))
            return 7; //no opponent's piece to between from and to
    }
    if (isMan(piece_from))
    {
        if (((player==BLACK)&&((to-from)<3)) || ((player==WHITE)&&((from-to)<3)))
            return 8; // wrong direction
    }

    //for now move is legal

    result = 0;
    if (isMan(piece_from) && (((player==BLACK)&&(ver[to]==8)) || ((player==WHITE)&&(ver[to]==1))))
        result += 100; /* man promotion */

    //executing move
    board[ver[to]][hor[to]] = board[ver[from]][hor[from]];
    if (result==100)
        board[ver[to]][hor[to]] += 2; // promote to king
    board[ver[from]][hor[from]] = EMPTY_F;

    if (CAPTURE==1)
    {
        board[(ver[from] + ver[to])/2][(hor[from] + hor[to])/2] = EMPTY_F;
    }
    lastMovedPieceType = board[ver[to]][hor[to]];
    return result;
}

int CheckersBoard::getOpponent(int player) const
{
    return 1-player;
}

void CheckersBoard::displayFields() const
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (fields[i][j] == -2) {
                std::cout << std::setw(3) << "**";
            } else if (fields[i][j] == -1) {
                std::cout << std::setw(3) << " ";
            } else {
                std::cout << std::setw(3) << fields[i][j];
            }
        }
        std::cout << std::endl;
    }
}

int CheckersBoard::getVer(int index) const
{
    return ver[index];
}

int CheckersBoard::getHor(int index) const
{
   return hor[index];
}

bool CheckersBoard::isManMoved() const {
    return lastMovedPieceType == BLACK_M || lastMovedPieceType == WHITE_M;
}

int CheckersBoard::getFieldSymbol(int row, int col) const
{
    return board[row][col] - 50;
}

int CheckersBoard::getPieceAt(int field) const
{

    if (field < 1 || field > 32) {
        return INVALID;  // Or another appropriate error value
    }


    int row = ver[field];
    int col = hor[field];


    return board[row][col];
}

std::vector<std::pair<int, int>> CheckersBoard::generateMoves(Owner player) const
{
    std::vector<std::pair<int, int>> moves;

    for (int i = 1; i <= 32; ++i)
    {
        int piece = getPieceAt(i);
        if (isPiece(piece) && getOwner(piece) == player) {
            for (int j = 1; j <= 32; ++j) {
                if (isLegalMove(player, i, j))
                {
                    moves.emplace_back(i, j);
                }
            }
        }
    }

    return moves;
}



bool CheckersBoard::isLegalMove(int player, int from, int to) const
{
    int result;
    int piece_mid; /* tylko przy biciu */
    int piece_from=board[ver[from]][hor[from]];
    int field_to=board[ver[to]][hor[to]];

    int CAPTURE;
    if (abs(getVer(from) - getVer(to)) == 2 && abs(getHor(from) - getHor(to)) == 2)
        CAPTURE = 1;
    else
        CAPTURE = 0;

    if ((from<1) || (from>32))
        return false; //from invalid feild
    if ((to<1) || (to>32))
        return false; //field to ivalid
    if ((!isPiece(piece_from)) || (getOwner(piece_from) != player))
        return false; //no player's piece on 'from' field
    if (field_to != EMPTY_F)
        return false; //field 'to' is not empty
    if ((CAPTURE==0)&&((abs(ver[from]-ver[to])!=1)||(abs(hor[from]-hor[to])!=1)))
        return false; //wrong geometry of move
    if ((CAPTURE==1)&&((abs(ver[from]-ver[to])!=2)||(abs(hor[from]-hor[to])!=2)))
        return false; //wrong geometry of move
    if (CAPTURE==1)
    {
        piece_mid=board[(ver[from]+ver[to])/2][(hor[from]+hor[to])/2];
        if ((!isPiece(piece_mid)) || (getOwner(piece_mid) != getOpponent(player)))
            return false; //no opponent's piece to between from and to
    }
    if (isMan(piece_from))
    {
        if (((player==BLACK)&&((to-from)<3)) || ((player==WHITE)&&((from-to)<3)))
            return false; // wrong direction
    }

    bool captureRequired = false;
    for (int i = 1; i <= 32; ++i) {
        int piece = getPieceAt(i);
        if (isPiece(piece) && getOwner(piece) == player)
        {
            if (canCapture(i)) {
                captureRequired = true;
                break;
            }
        }
    }

    if (captureRequired && CAPTURE == 0)
    {
        return false;
    }

    return true;

}

