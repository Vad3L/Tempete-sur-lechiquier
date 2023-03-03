#include <cstdlib>

#include <gf/SharedGraphics.h>
#include <gf/Sleep.h>

#include "bits/client/GameHub.h"
#include "bits/client/Singletons.h"
#include "bits/client/GameModel.h"

#include "bits/common/Network.h"
#include "bits/common/Constants.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    
	tsl::GameHub hub;
	tsl::Network network;  
	
	hub.getWindow().toggleFullscreen();
	
	gf::SingletonStorage<tsl::AudioManager> storageForResourceManager(tsl::gAudioManager);
  	tsl::gAudioManager().addSearchDir({ tsl::GAME_DATADIR });
	
	std::thread loading([&hub, &network]() {
		gf::SharedGraphics glContext(hub.getWindow());	
		hub.loadingOtherAssets(network);
	});

	loading.detach();
	
	// Background music
	tsl::gBackgroundMusic.setBuffer(tsl::gAudioManager().getSound("sounds/MainSounds.ogg"));
	tsl::gBackgroundMusic.setLoop(true);
	tsl::gBackgroundMusic.setVolume(tsl::BackgroundAmbiantVolume);
	tsl::gBackgroundMusic.play();
	

	hub.pushScene(*hub.download);
	hub.run();
	
  return EXIT_SUCCESS;
}