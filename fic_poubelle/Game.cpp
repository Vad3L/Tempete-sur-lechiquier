#include "Game.hpp"

#include <gf/Sleep.h>
#include <gf/Action.h>
#include <cassert>

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

	assert(repPartie.err == CodeRep::NONE);

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
	bool click = false;

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
		
		if(clickAction.isActive()) {
			click = true;
		}else {
			click = false;
		}
		
		//gf::Time time = clock.restart();

		if(myTurn && click) { // myTurn
			gf::Vector2i v = vue.transformInSelectedCase(event.mouseButton.coords);
			bool coupPionEnded = plateau.setMovement(myColor, v);

			if(coupPionEnded) {
	
				CoupReq coup;
				coup.posStart.x = plateau.coordCaseSelected.x;
				coup.posStart.y = plateau.coordCaseSelected.y;
				coup.posEnd.x = v.x;
				coup.posEnd.y = v.y;
				
				network.send(coup);
				
				network.queue.wait(packet);
				
				assert(packet.getType() == CoupRep::type);
				
				auto coupRep = packet.as<CoupRep>();
				
				// move piece
				if(coupRep.err == CodeRep::NONE) { // coup valide
					
					std::cout << "------COUP MOI------" << std::endl;
					plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece.isMoved = true;
					plateau.movePieces(plateau.coordCaseSelected, v);
					ChessColor colAdv = (myColor == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
					plateau.playerInEchec = plateau.isInEchec(colAdv);
					plateau.prettyPrint();

					plateau.coordCaseSelected = gf::Vector2i(-1,-1);
					plateau.moveAvailable.clear();	
					
					plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
					plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
					
					myTurn = !myTurn;
					plateau.prisePassant = false;
				}else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
					std::cout << "------COUP MOI INVALIDE------" << std::endl;
				}
			}
		}else {
			
			if(network.queue.poll(packet)) {
				
				//network.queue.wait(packet);
				assert(packet.getType() == CoupRep::type);
				
				auto coupAdv = packet.as<CoupRep>();
				
				if(coupAdv.err == CodeRep::NONE) { // coup valide
					
					std::cout << "------COUP ADVERSE------" << std::endl;
					plateau.state[coupAdv.posStart.y * 8 + coupAdv.posStart.x].piece.isMoved = true;
					plateau.movePieces(gf::Vector2i(coupAdv.posStart.x, coupAdv.posStart.y), gf::Vector2i(coupAdv.posEnd.x, coupAdv.posEnd.y));
					plateau.playerInEchec = plateau.isInEchec(myColor);
					plateau.prettyPrint();

					plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posStart.x,coupAdv.posStart.y));
					plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posEnd.x,coupAdv.posEnd.y));
						
					myTurn = !myTurn;	
					plateau.prisePassant = false;
				}else if(coupAdv.err == CodeRep::COUP_NO_VALIDE) {
					std::cout << "------COUP ADVERSE INVALIDE------" << std::endl;
				}
			}
		}

		
		vue.renderer.clear(gf::Color::Gray(0.5));
		vue.draw(plateau, myTurn);
		vue.renderer.display();

		actions.reset();
	}
}