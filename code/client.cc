#include <cstdlib>

#include <gf/SharedGraphics.h>
#include <gf/Sleep.h>

#include "bits/client/GameHub.h"
#include "bits/common/Network.h"

int main(int argc, char *argv[]) {
    
	tsl::GameHub hub;
	tsl::Network network;  
	
	hub.loadingDownloadAssets();
	hub.getWindow().toggleFullscreen();
	
	std::thread loading([&hub, &network]() {
		gf::SharedGraphics glContext(hub.getWindow());	
		hub.loadingOtherAssets(network);
	});

	loading.detach();
	//gf::sleep(gf::seconds(0.5));
	
	hub.pushScene(*hub.download);
	hub.run();
	
  return EXIT_SUCCESS;
}