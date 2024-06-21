#include "ComputerPlayer.h"
#include <random>
#include "Player.h"

int ComputerPlayer::evaluateBoard(const CheckersBoard& board)
{
    int myRating = 0;
    int opponentsRating = 0;

    for (int i = 1; i <= 32; ++i)
    {
        int piece = board.getPieceAt(i);
        if (board.isPiece(piece))
        {
            int value = 0;
            //king's value
            if (board.isKing(piece))
            {
                value = 15;
            }
            else
            {
                //pawn distance from promotion
                if (color == WHITE)
                {
                    value = 5 + (9 - board.getRow(i));
                }
                else if (color == BLACK)
                {
                    value = 5 + board.getRow(i);
                }
            }

            // center control
            int row = board.getRow(i);
            int col = board.getCol(i);
            if ((row >= 3 && row <= 6) && (col >= 3 && col <= 6))
            {
                value += 1;
            }
            // safe position
            if (col == 1 || col == 8)
            {
                value += 1;
            }


            if (board.getOwner(piece) == color)
            {
                myRating += value;
            }
            else
            {
                opponentsRating += value;
            }
        }
    }

    return myRating - opponentsRating;
}

int ComputerPlayer::alphaBeta(CheckersBoard& board, int depth, int alpha, int beta, bool maximizingPlayer)
{
    if (depth == 0 || board.isGameOver(color) || board.isGameOver(board.getOpponent(color)))
        return evaluateBoard(board);

    if (maximizingPlayer)
    {
        int maxEval = -10000;
        std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(color);

        for (const auto& move : possibleMoves)
        {
            int from = move.first;
            int to = move.second;
            bool isCapture = (std::abs(board.getVer(from) - board.getVer(to)) == 2 && std::abs(board.getHor(from) - board.getHor(to)) == 2);

            std::vector<int> capturedPositions;
            std::vector<int> capturedPieces;
            int promotedPiece = -1;

            if (isCapture)
            {
                int capturePos = (board.getVer(from) + board.getVer(to)) / 2 * 4 + (board.getHor(from) + board.getHor(to)) / 2;
                capturedPositions.push_back(capturePos);
                capturedPieces.push_back(board.getPieceAt((from + to) / 2));
            }

            if (board.isMan(board.getPieceAt(from)) && (((color == BLACK) && (board.getVer(to) == 8)) || ((color == WHITE) && (board.getVer(to) == 1))))
            {
                promotedPiece = board.getPieceAt(from);
            }

            board.makeMove(color, from, to, isCapture);
            int eval = alphaBeta(board, depth - 1, alpha, beta, false);
            board.undoMove(color, from, to, isCapture, capturedPositions, capturedPieces, promotedPiece);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if (beta < alpha)
                break; // Pruning occurs here
        }
        return maxEval;
    } else
    {
        int minEval = 10000;
        std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(board.getOpponent(color));

        for (const auto& move : possibleMoves)
        {
            int from = move.first;
            int to = move.second;
            bool isCapture = (std::abs(board.getVer(from) - board.getVer(to)) == 2 && std::abs(board.getHor(from) - board.getHor(to)) == 2);

            std::vector<int> capturedPositions;
            std::vector<int> capturedPieces;
            int promotedPiece = -1;

            if (isCapture)
            {
                int capturePos = (board.getVer(from) + board.getVer(to)) / 2 * 4 + (board.getHor(from) + board.getHor(to)) / 2;
                capturedPositions.push_back(capturePos);
                capturedPieces.push_back(board.getPieceAt((from + to) / 2));
            }

            if (board.isMan(board.getPieceAt(from)) && (((board.getOpponent(color) == BLACK) && (board.getVer(to) == 8)) || ((board.getOpponent(color) == WHITE) && (board.getVer(to) == 1))))
            {
                promotedPiece = board.getPieceAt(from);
            }

            board.makeMove(board.getOpponent(color), from, to, isCapture);
            int eval = alphaBeta(board, depth - 1, alpha, beta, true);
            board.undoMove(board.getOpponent(color), from, to, isCapture, capturedPositions, capturedPieces, promotedPiece);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if (beta < alpha)
                break; // Pruning occurs here
        }
        return minEval;
    }
}


std::string ComputerPlayer::sendMove(CheckersBoard& board, int depth, unsigned int random_seed)
{
    std::mt19937 rng;
    if (random_seed == 0)
    {
        std::random_device rd;
        rng.seed(rd());
    }
    else
    {
        rng.seed(random_seed);
    }

    int bestMoveFrom = -1;
    int bestMoveTo = -1;
    int bestValue = -10000;

    std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(color);
    std::vector<std::pair<int, int>> bestMoves;

    for (const auto& move : possibleMoves)
    {
        CheckersBoard newBoard = board;
        int from = move.first;
        int to = move.second;
        bool isCapture = (std::abs(newBoard.getVer(from) - newBoard.getVer(to)) == 2 && std::abs(newBoard.getHor(from) - newBoard.getHor(to)) == 2);

        newBoard.makeMove(color, from, to, isCapture);
        int boardValue = alphaBeta(newBoard, depth, -10000, 10000, false);  // Pass random_seed here

        if (boardValue > bestValue)
        {
            bestValue = boardValue;
            bestMoves.clear();
            bestMoves.push_back(move);
        }
        else if (boardValue == bestValue)
        {
            bestMoves.push_back(move);
        }
    }

    std::string moveNotation;
    if (!bestMoves.empty())
    {
        std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
        int randomIndex = dist(rng);
        bestMoveFrom = bestMoves[randomIndex].first;
        bestMoveTo = bestMoves[randomIndex].second;

        bool isCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2);
        moveNotation += std::to_string(bestMoveFrom) + (isCapture ? "x" : "-") + std::to_string(bestMoveTo);

        int result = board.makeMove(color, bestMoveFrom, bestMoveTo, isCapture);
        bool wasPromoted = (result == 100);


        while (isCapture && !wasPromoted && board.canCapture(bestMoveTo))
        {
            std::vector<std::pair<int, int>> additionalMoves = board.generateMoves(color);
            bestMoveFrom = bestMoveTo;
            bestValue = -10000;
            bestMoves.clear();

            for (const auto& move : additionalMoves)
            {
                int newTo = move.second;
                bool newCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(newTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(newTo)) == 2);

                if (newCapture)
                {
                    CheckersBoard newBoard = board;
                    newBoard.makeMove(color, bestMoveFrom, newTo, newCapture);
                    int boardValue = alphaBeta(newBoard, depth, -10000, 10000, false);  // Pass random_seed here

                    if (boardValue > bestValue)
                    {
                        bestValue = boardValue;
                        bestMoves.clear();
                        bestMoves.push_back(move);
                    }
                    else if (boardValue == bestValue)
                    {
                        bestMoves.push_back(move);
                    }
                }
            }

            if (!bestMoves.empty())
            {
                std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
                int randomIndex = dist(rng);
                bestMoveTo = bestMoves[randomIndex].second;
                isCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2);
                moveNotation += "x" + std::to_string(bestMoveTo);
                board.makeMove(color, bestMoveFrom, bestMoveTo, isCapture);
            }
            else
                break;
        }
    }
    return moveNotation;
}

void ComputerPlayer::makeMove(CheckersBoard& board, int depth, unsigned int random_seed)
{
    std::mt19937 rng;
    if (random_seed == 0)
    {
        std::random_device rd;
        rng.seed(rd());
    }
    else
    {
        rng.seed(random_seed);
    }

    int bestMoveFrom = -1;
    int bestMoveTo = -1;
    int bestValue = -10000;

    std::vector<std::pair<int, int>> possibleMoves = board.generateMoves(color);
    std::vector<std::pair<int, int>> bestMoves;

    for (const auto& move : possibleMoves)
    {
        CheckersBoard newBoard = board;
        int from = move.first;
        int to = move.second;
        bool isCapture = (std::abs(newBoard.getVer(from) - newBoard.getVer(to)) == 2 && std::abs(newBoard.getHor(from) - newBoard.getHor(to)) == 2);

        newBoard.makeMove(color, from, to, isCapture);
        int boardValue = alphaBeta(newBoard, depth, -10000, 10000, false);

        if (boardValue > bestValue)
        {
            bestValue = boardValue;
            bestMoves.clear();
            bestMoves.push_back(move);
        }
        else if (boardValue == bestValue)
        {
            bestMoves.push_back(move);
        }
    }

    std::string moveNotation;
    if (!bestMoves.empty())
    {
        std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
        int randomIndex = dist(rng);
        bestMoveFrom = bestMoves[randomIndex].first;
        bestMoveTo = bestMoves[randomIndex].second;

        bool isCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2);
        moveNotation += std::to_string(bestMoveFrom) + (isCapture ? "x" : "-") + std::to_string(bestMoveTo);

        board.makeMove(color, bestMoveFrom, bestMoveTo, isCapture);

        // Check for additional captures
        while (isCapture && !board.isKing(bestMoveTo) && board.canCapture(bestMoveTo))
        {
            std::vector<std::pair<int, int>> additionalMoves = board.generateMoves(color);
            bestMoveFrom = bestMoveTo;
            bestValue = -10000;
            bestMoves.clear();

            for (const auto& move : additionalMoves)
            {
                int newTo = move.second;
                bool newCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(newTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(newTo)) == 2);

                if (newCapture)
                {
                    CheckersBoard newBoard = board;
                    newBoard.makeMove(color, bestMoveFrom, newTo, newCapture);
                    int boardValue = alphaBeta(newBoard, depth, -10000, 10000, false);  // Pass random_seed here

                    if (boardValue > bestValue)
                    {
                        bestValue = boardValue;
                        bestMoves.clear();
                        bestMoves.push_back(move);
                    }
                    else if (boardValue == bestValue)
                    {
                        bestMoves.push_back(move);
                    }
                }
            }

            if (!bestMoves.empty())
            {
                std::uniform_int_distribution<int> dist(0, bestMoves.size() - 1);
                int randomIndex = dist(rng);
                bestMoveTo = bestMoves[randomIndex].second;
                isCapture = (std::abs(board.getVer(bestMoveFrom) - board.getVer(bestMoveTo)) == 2 && std::abs(board.getHor(bestMoveFrom) - board.getHor(bestMoveTo)) == 2);
                moveNotation += "x" + std::to_string(bestMoveTo);
                board.makeMove(color, bestMoveFrom, bestMoveTo, isCapture);
            }
            else
                break;
        }
    }
    std::cout << "Computer's move: " << moveNotation << std::endl;

}