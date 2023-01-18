#include <iostream>
#include <thread>
#include "GameHub.hpp"	

#include <gf/SharedGraphics.h>

int main(int argc, char* argv[]) {

	GameHub hub;

	auto loading = std::thread([&hub]() {
		gf::SharedGraphics glContext(hub.getWindow());
		hub.loadingDownloadAssets();
		hub.loadingAssets();
	});

	loading.detach();

	while(!hub.loadingFinish()) {
		
	}
	hub.pushScene(*hub.start);
	hub.run();
	

	return EXIT_SUCCESS;
}
