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

#include <string>

#include "../model/Plateau.hpp"

class Vue {
private:
	gf::Vector2f beginBoard;
	int sizeCircle;
	float sizeSquare;
	int myColor;

public:
	gf::Texture sheet;
	gf::Vector2u ScreenSize;
	gf::Window window;
	gf::RenderWindow renderer;

	gf::LockedView mainView;
	gf::ScreenView screenView;
	gf::ViewContainer views;

	Vue(gf::Vector2u SSize, int mycolor);
	void set_side (int color);
	void print(Plateau p);
};

#endif // VUE_H
