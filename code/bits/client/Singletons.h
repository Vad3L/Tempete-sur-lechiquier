#ifndef TSL_SINGLETONS_H
#define TSL_SINGLETONS_H

#include <gf/Singleton.h>

#include "AudioManager.h"

namespace tsl {

    inline float BackgroundAmbiantVolume = 6.0f;
    inline float FxsVolume = 74.0f;

    extern gf::Singleton<AudioManager> gAudioManager;
    extern sf::Sound gBackgroundMusic;

}
#endif // TSL_SINGLETONS_H