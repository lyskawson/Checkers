#include "CheckersBoard.h"
#include "Player.h"
#include "Controller.h"
#include "View.h"

/*
    make_move function:
    0 - valid move (normal or capture)
    100 - valid move + pawn promotion
    1 - illegal move attempt - invalid 'from' square
    2 - illegal move attempt - invalid 'to' square
    3 - illegal move attempt - no player piece on 'from' square
    4 - illegal move attempt - 'to' square occupied
    5 - illegal normal move attempt - move geometry error
    6 - illegal capture move attempt - move geometry error
    7 - illegal capture move attempt - no opponent piece between 'from' and 'to'
    8 - illegal normal/capture move attempt - pawn moving backwards

    main program:
    9 - illegal normal move attempt - player has a capture available
    10 - illegal capture attempt - backward capture after pawn promotion
    11 - illegal capture attempt - further capture possible from the final square
    12..19 - codes reserved for other cases of invalid moves

    0 - normal game result, displayed on stdout. BW (black wins) or WW (white wins)
       (capture all opponent pieces, or opponent has no legal move)
    1-19 - loss due to an attempt to make an illegal move
    20 - loss due to failure to make a move in the given time
    21 - loss due to an attempt to move out of turn
    22 - loss due to closing the connection with the broker
    40 - draw - 40 moves by kings without a capture
    41 - draw - game time exceeded

 */





int main()
{
    CheckersBoard board;
    PlayerType whiteType = PlayerType::Human;
    PlayerType blackType = PlayerType::Computer;

    Controller controller(board, whiteType, blackType);
    View view(board);
    controller.playGame();

    /*sf::RenderWindow window(sf::VideoMode(1300, 1050), "Checkers");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            controller.playGameSFML(event);  // Call playGameSFML here
        }

        window.clear();
        view.draw(window);
        window.display();
    }*/
    return 0;




}