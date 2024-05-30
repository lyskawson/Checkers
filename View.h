
#include "SFML/Graphics.hpp"
#include "CheckersBoard.h"

#ifndef CHECKERS_VIEW_H
#define CHECKERS_VIEW_H

class View
{

    CheckersBoard & board;
    sf::Font font;
    sf::Text level[4], scoreboard, exit, timer_text, score, diff, scoreboardT, scoreboardt, textButton;
    sf::Texture blackM, whiteM, blackK, whiteK, boardTexture, backgroundTexture;
    sf::Sprite blackMan, whiteMan, blackKing, whiteKing, chessboard, background;




public:

    View(CheckersBoard & board);
    void draw(sf::RenderWindow & window);



};


#endif //CHECKERS_VIEW_H
