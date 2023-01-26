#ifndef SINGLETONS_H
#define SINGLETONS_H

#include <gf/Singleton.h>

#include "AudioManager.hpp"

static float BackgroundAmbiantVolume = 74.0f;
static constexpr float FxsVolume = 80.0f;

extern gf::Singleton<AudioManager> gAudioManager;
extern sf::Sound gBackgroundMusic;

#endif // SINGLETONS_H