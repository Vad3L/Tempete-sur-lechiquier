#ifndef TSL_TOOLS_H
#define TSL_TOOLS_H

#include <gf/RenderTarget.h>
#include <gf/Coordinates.h>
#include <SFML/Audio.hpp>

#include "Singletons.h"

namespace tsl {
    
    inline sf::Sound m_clickButton;
    
	inline void playClickButton() {
		m_clickButton.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
        m_clickButton.setVolume(FxsVolume);
		m_clickButton.play();
    }
}

#endif // TSL_TOOLS_H