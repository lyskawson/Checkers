#ifndef CHECKERS_CHECKERSBOARD_H
#define CHECKERS_CHECKERSBOARD_H

#include "Array2D.h"
#include <iostream>

enum class squareState {
    empty,
    whiteMan,
    blackMan,
    whiteKing,
    blackKing
};

struct Field {
    squareState state;
    Field() : state(squareState::empty) {}
};

enum GameState { RUNNING, FINISHED_WIN, FINISHED_LOSS };


class CheckersBoard
{

    Array2D<Field> board{1,1};


public:
    CheckersBoard();
    void restartBoard();
    void displayBoard() const;
    bool isGameOver() const;
    bool canMove(int row, int col) const;
    bool checkForNormalMoves(int row, int col) const;
    bool checkForCaptureMoves(int row, int col) const;
    bool hasMandatoryCapture(int row, int col) const;
    bool isCaptureMove(int row, int col, int new_row, int new_col) const;
    bool isLegalMove(int row, int col, int new_row, int new_col) const;
    void executeMove(int row, int col, int new_row, int new_col);
    bool canContinueCapture(int row, int col) const;
    


};


#endif //CHECKERS_CHECKERSBOARD_H
