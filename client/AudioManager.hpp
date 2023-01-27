#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


#include <gf/ResourceManager.h>
#include <SFML/Audio.hpp>

class AudioManager : public gf::ResourceManager {
	public:
		AudioManager();
		sf::SoundBuffer& getSound(const gf::Path &path);
		sf::Music& getMusic(const gf::Path &path);

	private:
		gf::ResourceCache<sf::SoundBuffer> m_sounds;
		gf::ResourceCache<sf::Music> m_musics;
};


#endif // AUDIO_MANAGER_H
