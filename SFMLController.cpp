#include "SFMLController.h"


SFMLController::SFMLController(CheckersBoard& board, PlayerType whiteType, PlayerType blackType) : board(board)
{
    whitePlayer = createPlayer(whiteType, Owner::WHITE);
    blackPlayer = createPlayer(blackType, Owner::BLACK);
}



std::unique_ptr<Player> SFMLController::createPlayer(PlayerType type, Owner color)
{
    if (type == PlayerType::Human)
    {
        return std::make_unique<SFMLPlayer>(color, board);
    }
    else
    {
        return std::make_unique<ComputerPlayer>(color, board);
    }
}

void SFMLController::playGame(sf::Event &event) {
    static int drawCounter = 0;
    static int moveNum = 0;
    const int drawLimit = 40;
    static int player = 1;

    if (drawCounter > drawLimit) {
        std::cout << "Exceeded limit of moves by kings without progress - draw" << std::endl;
        std::cout << "DRAW" << std::endl;
        std::exit(40);
    }

    if (player == 0) {
        whitePlayer->makeMove(board, event);
    } else {
        blackPlayer->makeMove(board, event);
    }

    if (board.isManMoved()) {
        drawCounter = 0;
    }

    // Only increment moveNum and switch player if a valid move is made
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        drawCounter++;
        moveNum++;
        player = moveNum % 2;

        if (board.isGameOver(1 - player)) {
            std::cout << "Winner: " << (player == 0 ? "Black" : "White") << std::endl;
            std::exit(0);  // end the game
        }
    }
}