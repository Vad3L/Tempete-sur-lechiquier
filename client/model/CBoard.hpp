#ifndef CBOARD_H
#define CBOARD_H

#include <gf/Vector.h>
#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/EntityContainer.h>
#include <gf/Clock.h>
#include <gf/Action.h>

#include <string>

#include "../../model/Plateau.hpp"
#include "CPiece.hpp"

struct cmpByVector2i {
	bool operator()(const gf::Vector2i& a, const gf::Vector2i& b) const {
		if ((a.x == b.x)) {
			return(a.y < b.y);
		}
		return(a.x < b.x);
	}
};

class CBoard {
	private:
		;
		
		gf::Vector2f beginBoard;
		int sizeCircle;
		float sizeSquare;
		int myColor = 1;

		
	public:
		Plateau board;
		gf::Vector2u ScreenSize;
		gf::Window window;
		gf::RenderWindow renderer;

		gf::EntityContainer MainEntity;
		
		std::vector<CPiece> pieces;

		gf::LockedView mainView;
		gf::ScreenView screenView;
		gf::ViewContainer views;

		CBoard(gf::Vector2u SSize, const std::string& gName, int mycolor);

		void initBoard();
		void print();
		void printBoard();
};

#endif //CBOARD_H