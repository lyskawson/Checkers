#include "View.h"
#include <fstream>
#include <iostream>
#include <vector>

View::View(CheckersBoard & board) : board(board)
{

    //load textures form file
    blackK.loadFromFile("../textures/blackKing.png");
    whiteK.loadFromFile("../textures/whiteKing.png");
    blackM.loadFromFile("../textures/blackMan.png");
    whiteM.loadFromFile("../textures/whiteMan.png");
    boardTexture.loadFromFile("../textures/board.png");
    backgroundTexture.loadFromFile("../textures/background.png");

    //set textures on sprites
    blackMan.setTexture(blackM);
    whiteMan.setTexture(whiteM);
    blackKing.setTexture(blackK);
    whiteKing.setTexture(whiteK);
    chessboard.setTexture(boardTexture);
    background.setTexture(backgroundTexture);

    //scale sprites
    double scale = 0.108333;
    blackMan.setScale(sf::Vector2f(scale, scale));
    whiteMan.setScale(sf::Vector2f(scale, scale));
    blackKing.setScale(sf::Vector2f(scale, scale));
    whiteKing.setScale(sf::Vector2f(scale, scale));
    chessboard.setScale(sf::Vector2f(0.65,0.65));

    background.setScale(sf::Vector2f(1,1.1));


}

void View::draw (sf::RenderWindow & window)
{
    float y = 62.0f;
    float x = 60.0f;
    background.setPosition(0,0);
    chessboard.setPosition(x,57);
    window.draw(background);
    window.draw(chessboard);



    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            float size = 117.0f;

            int fieldSymbol = board.getFieldSymbol(i, j);

            switch (fieldSymbol) {
                case 1:
                    blackMan.setPosition(size * j - x, size * i-y);
                    window.draw(blackMan);
                    break;
                case 2:
                    whiteMan.setPosition(size * j-x, size * i-y);
                    window.draw(whiteMan);
                    break;
                case 3:
                    blackKing.setPosition(size * j-x, size * i-y);
                    window.draw(blackKing);
                    break;
                case 4:
                    whiteKing.setPosition(size * j-x, size * i-y);
                    window.draw(whiteKing);
                    break;
                default:
                    break;
            }
        }
    }

}


