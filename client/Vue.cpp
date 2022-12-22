#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, const std::string& gName) : window(gName, SSize), renderer(window) {
    ScreenSize = gf::Vector2u(1000, 1000);
    sizeSquare = 50.0f;
    beginBoard = gf::Vector2f(300.0f, 300.0f);
    gf::Vector2f boardSize(sizeSquare * 10.0f, sizeSquare * 10.0f);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    //mainView = gf::LockedView(ScreenSize / 2, boardSize + 50.0f);

   // views.addView(mainView);
  //  views.addView(screenView);
  //  views.setInitialFramebufferSize(ScreenSize);



}

