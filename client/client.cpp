#include <iostream>
#include <thread>

#include "GameHub.hpp"  
#include "Network.hpp"


#include <gf/SharedGraphics.h>

int main(int argc, char* argv[]) {

	GameHub hub;
	Network network;
	hub.getWindow().toggleFullscreen();

	auto loading = std::thread([&hub, &network]() {
		gf::SharedGraphics glContext(hub.getWindow());
		hub.loadingDownloadAssets();
		hub.loadingAssets(network);
	});

	loading.detach();

	gf::sleep(gf::milliseconds(1000));
	hub.pushScene(*hub.download);
	hub.run();
	return EXIT_SUCCESS;

}
