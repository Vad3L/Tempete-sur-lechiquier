#include "Game.hpp"

#include <gf/Sleep.h>
#include <cassert>

#include "../protocole/protocole.h"
#include <iostream>

Game::Game(char* argv[]) : vue(gf::Vector2u(1000, 1000), 0) {
	
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

	couleur = repPartie.coulPion;

	if (couleur == 1) {
		myTurn = true;
	}
	else {
		myTurn = false;
	}

	std::cout << "Vous jouez la couleur : " << couleur << std::endl;
	vue.set_side(couleur);

	// boucle de jeu

	while (vue.window.isOpen()) {

		vue.renderer.clear();
		vue.print(plateau);
		vue.renderer.display();
	}
}
