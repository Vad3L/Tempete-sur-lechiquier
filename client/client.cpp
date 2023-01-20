#include <iostream>
#include <thread>

#include "GameHub.hpp"  
#include "Network.hpp"


#include <gf/SharedGraphics.h>

int main(int argc, char* argv[]) {

	GameHub hub;
	Network network;
	hub.getWindow().toggleFullscreen();

	hub.loadingDownloadAssets();
	hub.loadingAssets(network);

	hub.pushScene(*hub.download);
	hub.run();
	return EXIT_SUCCESS;

}
