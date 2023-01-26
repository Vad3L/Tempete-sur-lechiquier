#include <iostream>
#include <thread>

#include "GameHub.hpp"  
#include "Network.hpp"
#include "GameData.hpp"

#include "Singletons.hpp"

#include <gf/SharedGraphics.h>

int main() {

	GameHub hub;
	Network network;
	GameData data;

	gf::SingletonStorage<AudioManager> storageForAudioManager(gAudioManager);
	gAudioManager().addSearchDir({GAME_DATADIR});
	BackgroundAmbiantVolume = data.m_sounds;

	hub.getWindow().toggleFullscreen();

	hub.loadingDownloadAssets();
	hub.loadingAssets(network, data);

	// Background music
	gBackgroundMusic.setBuffer(gAudioManager().getSound("sounds/MainSounds.ogg"));
	gBackgroundMusic.setLoop(true);
	gBackgroundMusic.setVolume(BackgroundAmbiantVolume);
	gBackgroundMusic.play();
	
	hub.pushScene(*hub.download);
	hub.run();
	return EXIT_SUCCESS;

}
