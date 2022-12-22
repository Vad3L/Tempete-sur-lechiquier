#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, int mycolor) : window("Temepete sur l'echiquier", SSize),
renderer(window), 
myColor(mycolor) {
    ScreenSize = gf::Vector2u(1000, 1000);
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

void Vue::print(Plateau p) {
    
    renderer.setView(mainView);

    gf::Color4f peru = gf::Color::fromRgba32(205, 133, 63);
    gf::Color4f saddlebrown = gf::Color::fromRgba32(139, 69, 19);

    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    shape.setAnchor(gf::Anchor::Center);
    

    //affichage plateau 
    for  (Case &c : p.state) {
        int x = c.position.x;
        int y = c.position.y;

        shape.setPosition(gf::Vector2f(beginBoard.height + ((float)x * sizeSquare), beginBoard.col + ((float)y * sizeSquare)));
        if (y % 2 == 0) {
            if (x % 2 == 0) {
                shape.setColor(peru);
            }
            else {
                shape.setColor(saddlebrown);
            }
        }
        else {
            if (x % 2 == 0) {
                shape.setColor(saddlebrown);
            }
            else {
                shape.setColor(saddlebrown);
            }
        }
        renderer.draw(shape);
    }

    renderer.setView(screenView);
    
}
