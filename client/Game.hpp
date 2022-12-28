#ifndef GAME_H
#define GAME_H

#include <gf/Event.h>

#include "Network.hpp"
#include "../model/Plateau.hpp"
#include "Vue.hpp"

class Game {
	private:
		bool myTurn;
		ChessColor myColor;

		Network network;
		Plateau plateau;
		Vue vue;
		
	public:
		Game(char* argv[]);
		void run();
};

#endif // GAME_H
