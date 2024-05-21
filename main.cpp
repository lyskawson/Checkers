#include "CheckersBoard.h"
#include "Player.h"
#include "Controller.h"

int main()
{
    CheckersBoard board;
    PlayerType whiteType = PlayerType::Human;
    PlayerType blackType = PlayerType::Human;

    Controller controller(board, whiteType, blackType);
    controller.playGame();



    return 0;
}