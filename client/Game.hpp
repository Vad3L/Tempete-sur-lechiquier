#ifndef GAME_H
#define GAME_H

#include "Network.hpp"

#include "../model/Plateau.hpp"
#include "Vue.hpp"

class Game {
	private:
		bool myTurn;
		int couleur;

		Network network;
		Plateau plateau;
		//View board;
		
	public:
		Game(char* argv[]);
		void run();
};

#endif // GAME_H
