#ifndef SINGLETONS_H
#define SINGLETONS_H

#include <gf/Singleton.h>

#include "AudioManager.hpp"

inline float BackgroundAmbiantVolume = 40.0f;
inline float FxsVolume = 74.0f;

extern gf::Singleton<AudioManager> gAudioManager;
extern sf::Sound gBackgroundMusic;

#endif // SINGLETONS_H