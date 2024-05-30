
#include "ComputerPlayer.h"
void ComputerPlayer::makeMove1(CheckersBoard& board)
{

}


void ComputerPlayer::makeMove(CheckersBoard& board)
{
    int bestMoveFrom = -1;
    int bestMoveTo = -1;
    int bestValue = -10000;


    std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(color);
    std::cout << "Possible moves: "<< std::endl;
    for (const auto& move : possibleMoves) {
        std::cout << "Possible move: " << move.first << " " << move.second << std::endl;
    }

    for (const auto& move : possibleMoves)
    {
        CheckersBoard newBoard = board;
        int from = move.first;
        int to = move.second;
        int CAPTURE = (abs(newBoard.getVer(from) - newBoard.getVer(to)) == 2 && abs(newBoard.getHor(from) - newBoard.getHor(to)) == 2) ? 1 : 0;

        newBoard.makeMove(color, from, to, CAPTURE);
        int boardValue = alphaBeta(newBoard, 5, -10000, 10000, false);

        if (boardValue > bestValue) {
            bestValue = boardValue;
            bestMoveFrom = from;
            bestMoveTo = to;
        }
    }

    if (bestMoveFrom != -1 && bestMoveTo != -1)
    {
        int CAPTURE = (abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2) ? 1 : 0;

        board.makeMove(color, bestMoveFrom, bestMoveTo, CAPTURE);

        // Check for additional captures
        while (CAPTURE == 1 && board.canCapture(bestMoveTo)) {
            std::vector<std::pair<int, int>> additionalMoves = board.generateMoves(color);
            bestMoveFrom = bestMoveTo;
            bestValue = -10000;
            bool captureMade = false;

            for (const auto& move : additionalMoves) {
                int newTo = move.second;
                int newCapture = (abs(board.getVer(bestMoveFrom) - board.getVer(newTo)) == 2 && abs(board.getHor(bestMoveFrom) - board.getHor(newTo)) == 2) ? 1 : 0;

                if (newCapture == 1) {
                    CheckersBoard newBoard = board;
                    newBoard.makeMove(color, bestMoveFrom, newTo, newCapture);
                    int boardValue = alphaBeta(newBoard, 5, -10000, 10000, false);

                    if (boardValue > bestValue) {
                        bestValue = boardValue;
                        bestMoveTo = newTo;
                        captureMade = true;
                    }
                }
            }

            if (captureMade) {
                CAPTURE = (abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2) ? 1 : 0;
                board.makeMove(color, bestMoveFrom, bestMoveTo, CAPTURE);
            } else {
                break;
            }
        }
    }
    else
        std::cout << "No moves available for computer player" << std::endl;

}

int ComputerPlayer::alphaBeta(CheckersBoard& board, int depth, int alpha, int beta, bool maximizingPlayer)
{
    if (depth == 0 || board.isGameOver(color) || board.isGameOver(board.getOpponent(color))) {
        return evaluateBoard(board);
    }

    if (maximizingPlayer)
    {
        int maxEval = -10000;
        std::vector<std::pair<int, int>> possibleMoves = board.generateMoves( color);

        for (const auto& move : possibleMoves)
        {
            CheckersBoard newBoard = board;
            int from = move.first;
            int to = move.second;
            int CAPTURE = (abs(newBoard.getVer(from) - newBoard.getVer(to)) == 2 && abs(newBoard.getHor(from) - newBoard.getHor(to)) == 2) ? 1 : 0;

            newBoard.makeMove(color, from, to, CAPTURE);
            int eval = alphaBeta(newBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else
    {
        int minEval = 10000;
        std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(color);

        for (const auto& move : possibleMoves)
        {
            CheckersBoard newBoard = board;
            int from = move.first;
            int to = move.second;
            int CAPTURE = (abs(newBoard.getVer(from) - newBoard.getVer(to)) == 2 && abs(newBoard.getHor(from) - newBoard.getHor(to)) == 2) ? 1 : 0;

            newBoard.makeMove(board.getOpponent(color), from, to, CAPTURE);
            int eval = alphaBeta(newBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

int ComputerPlayer::evaluateBoard(const CheckersBoard& board) {
    // Define piece values
    const int MAN_VALUE = 1;
    const int KING_VALUE = 3;

    // Initialize scores for both players
    int whiteScore = 0;
    int blackScore = 0;

    // Iterate over all board positions
    for (int i = 1; i <= 32; ++i) {
        int piece = board.getPieceAt(i);

        if (board.isPiece(piece)) {
            if (board.getOwner(piece) == Owner::WHITE) {
                if (board.isMan(piece)) {
                    whiteScore += MAN_VALUE;
                } else if (board.isKing(piece)) {
                    whiteScore += KING_VALUE;
                }
            } else if (board.getOwner(piece) == Owner::BLACK) {
                if (board.isMan(piece)) {
                    blackScore += MAN_VALUE;
                } else if (board.isKing(piece)) {
                    blackScore += KING_VALUE;
                }
            }
        }
    }

    // Return the difference in scores
    return whiteScore - blackScore;
}

