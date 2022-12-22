#ifndef VUE_H
#define VUE_H

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Vector.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Color.h>
#include <gf/Shapes.h>

#include <string>

#include "../model/Plateau.hpp"

class Vue {
private:
	gf::Vector2f beginBoard;
	int sizeCircle;
	float sizeSquare;
	int myColor;
public:

	gf::Vector2u ScreenSize;
	gf::Window window;
	gf::RenderWindow renderer;

	gf::LockedView mainView;
	gf::ScreenView screenView;
	gf::ViewContainer views;

	Vue(gf::Vector2u SSize, int mycolor);
	void print(Plateau p);
};

#endif // VUE_H
