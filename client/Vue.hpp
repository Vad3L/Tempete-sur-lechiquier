#ifndef VUE_H
#define VUE_H
#include <iostream>

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Vector.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Color.h>
#include <gf/Shapes.h>
#include <gf/Texture.h>
#include <gf/Sprite.h>


#include "../model/Plateau.hpp"

class Vue {
	private:
		gf::Vector2f beginBoard;
		float sizeSquare;
		ChessColor myColor;
		gf::Vector2f boardSize;
	public:
		gf::Texture sheetPiece;
		gf::Texture sheetNumberLetter;
		gf::Vector2u ScreenSize;
		gf::Window window;
		gf::RenderWindow renderer;

		gf::LockedView mainView;
		gf::ScreenView screenView;
		gf::ViewContainer views;

		
		Vue(gf::Vector2u SSize, ChessColor mycolor);
		void setColor (ChessColor color);
		void draw(Plateau p);
		gf::Vector2i transformInSelectedCase(gf::Vector2i mouseCoord);
};

#endif // VUE_H
