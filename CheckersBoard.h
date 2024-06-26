#ifndef CHECKERSENGINE_CHECKERSBOARD_H
#define CHECKERSENGINE_CHECKERSBOARD_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

enum BoardState {
    BLACK_M = 51,
    WHITE_M = 52,
    BLACK_K = 53,
    WHITE_K = 54,
    EMPTY_F = 55,
    INVALID = 56,
    OUTSIDE = 57
};

enum Owner { WHITE = 0, BLACK = 1 };


class CheckersBoard
{

    std::vector<std::vector<int>> board;
    std::vector<int> hor;
    std::vector<int> ver;
    std::vector<std::vector<int>> fields;
    int lastMovedPieceType;


public:
    CheckersBoard();
    void restartBoard();
    void displayBoard() const;
    int makeMove(int player, int from, int to, int CAPTURE);
    bool isGameOver(int player) const;
    bool canMove(int field) const; // checks if a piece in field can make a normal move
    Owner getOwner(int piece) const;
    bool isMan(int piece) const;
    bool isKing(int piece) const;
    bool canCapture(int field) const; // checks if a piece in field can make a capture move
    bool isPiece(int piece) const;
    int getOpponent(int player) const;
    int getVer(int field) const;
    int getHor(int field) const;
    bool isManMoved() const; //checks if was man moved in last move
    int getPieceAt(int field) const;
    std::vector<std::pair<int, int>> generateMoves(int player ) const; //generate all possible moves for ai
    bool isLegalMove(int player, int from, int to) const; //checks if move is legal
    std::vector<int> splitNotation(const std::string& notation, char delimiter); //helper for makeMovefromNotation
    bool makeMoveFromNotation(const std::string& notation, int player);
    int getRow(int position) const;
    void undoMove(int player, int from, int to, bool isCapture, const std::vector<int>& capturedPositions, const std::vector<int>& capturedPieces, int promotedPiece);
    int getCol(int position) const;
    void setPieceAt(int position, int piece);


};


#endif //CHECKERSENGINE_CHECKERSBOARD_H
