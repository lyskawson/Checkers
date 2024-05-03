#include "CheckersBoard.h"
#include "Player.h"
#include "Controller.h"

int main()
{
    PlayerType whiteType = PlayerType::Human;
    PlayerType blackType = PlayerType::Computer;
    CheckersBoard board1;
    board1.displayBoard();

    return 0;
}