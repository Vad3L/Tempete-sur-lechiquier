#include "CBoard.hpp"
#include <gf/Color.h>

CBoard::CBoard(gf::Vector2u SSize, const std::string& gName, int mycolor) : window(gName, SSize),
renderer(window),
myColor(mycolor),
board() {

    initBoard();

}

void CBoard::initBoard() {
    ScreenSize = gf::Vector2u(1000, 1000);
    sizeCircle = 10.0f;
    sizeSquare = 50.0f;
    beginBoard = gf::Vector2f(300.0f, 300.0f);
    gf::Vector2f boardSize(sizeSquare * 10.0f, sizeSquare * 10.0f);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    mainView = gf::LockedView(ScreenSize / 2, boardSize + 50.0f);

    views.addView(mainView);
    views.addView(screenView);
    views.setInitialFramebufferSize(ScreenSize);

    if (myColor == -1) {
        mainView.setRotation(gf::Pi);
    }

}

void CBoard::print() {
    renderer.clear();
    renderer.setView(mainView);
    printBoard();
    // draw piece
    for (auto& it : pieces) {
        it.render(renderer);
    }
    renderer.setView(screenView);
    renderer.display();
}

void CBoard::printBoard() {
    gf::Color4f peru = gf::Color::fromRgba32(205, 133, 63);
    gf::Color4f saddlebrown = gf::Color::fromRgba32(139, 69, 19);
    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            shape.setPosition(gf::Vector2f(beginBoard.height + ((float)j * sizeSquare), beginBoard.col + ((float)i * sizeSquare)));
            if (j % 2 == 0) { // ligne paire
                if (i % 2 == 0) { // clonne paire
                    shape.setColor(peru);
                }
                else {
                    shape.setColor(saddlebrown);
                }
            }
            else { // ligne // impair
                if (i % 2 == 0) { // colonne paire
                    shape.setColor(saddlebrown);
                }
                else {
                    shape.setColor(peru);
                }
            }
            shape.setAnchor(gf::Anchor::Center);
            renderer.draw(shape);
        }
    }
}