#ifndef VUE_H
#define VUE_H

#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Vector.h>
//#include <gf/Views.h>
//#include <gf/ViewContainer.h>

#include <string>

class Vue {
private:
	gf::Vector2f beginBoard;
	float sizeSquare;
public:

	gf::Vector2u ScreenSize;
	gf::Window window;
	gf::RenderWindow renderer;

	//gf::LockedView mainView;
	//gf::ScreenView screenView;
	//gf::ViewContainer views;

	Vue(gf::Vector2u SSize, const std::string& gName);

};

#endif // VUE_H