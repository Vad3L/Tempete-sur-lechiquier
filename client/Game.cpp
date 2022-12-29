#include "Game.hpp"

#include <gf/Sleep.h>
#include <gf/Action.h>
#include <cassert>

#include "../protocole/protocole.h"
#include <iostream>

Game::Game(char* argv[]) : vue(gf::Vector2u(1000, 1000), ChessColor::WHITE) {
	
	network.connect(argv[1], std::to_string(atoi(argv[2])));
	gf::sleep(gf::milliseconds(500));
	assert(network.isConnected());
}

void Game::run() {
	
	gf::Packet packet;
	network.queue.wait(packet);
	assert(packet.getType() == PartieRep::type);

	auto repPartie = packet.as<PartieRep>();

	assert(repPartie.err == TCodeRep::OK);

	myColor = repPartie.coulPion;

	if (myColor == ChessColor::WHITE) {
		myTurn = true;
	}
	else {
		myTurn = false;
	}

	std::cout << "Vous jouez la couleur : " << (int)myColor << std::endl;
	vue.setColor(myColor);

	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Q);
	actions.addAction(closeWindowAction);

	gf::Action clickAction("Click");
	clickAction.addMouseButtonControl(gf::MouseButton::Left);
	clickAction.setInstantaneous();
	actions.addAction(clickAction);

	gf::Action fullscreenAction("Fullscreen");
  	fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  	actions.addAction(fullscreenAction);
	
	// boucle de jeu
	gf::Clock clock;
	bool fullscreen = false;

	while (vue.window.isOpen()) {
		gf::Event event;
		
		while(vue.window.pollEvent(event)){
            actions.processEvent(event);
            vue.views.processEvent(event);
        }	
		
        if(closeWindowAction.isActive()){
            vue.window.close();
            break;
        }

		if (fullscreenAction.isActive()) {
			fullscreen = !fullscreen;
    		vue.window.setFullscreen(fullscreen);
    	}

		gf::Time time = clock.restart();

		if(true) { //myTurn
			if(clickAction.isActive()) {
				gf::Vector2i v = vue.transformInSelectedCase(event.mouseButton.coords);
				myTurn = plateau.setMovement(myColor, v);
			}
		}

		
		vue.renderer.clear(gf::Color::Gray(0.5));
		vue.draw(plateau);
		vue.renderer.display();

		actions.reset();
	}
}
