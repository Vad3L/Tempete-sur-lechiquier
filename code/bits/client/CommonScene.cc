#include "CommonScene.h"

#include "GameHub.h"
#include "Singletons.h"

#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Sprite.h>

namespace tsl {

	CommonScene::CommonScene(GameHub& game)
	: gf::Scene(game.getRenderer().getSize())
	, m_game(game)
	, m_quitAction("Quit")
	, m_fullscreenAction("Fullscreen")
	, m_backgroundTexture(m_game.resources.getTexture("images/StartMenu.jpg"))
	{
		setClearColor(gf::Color::Black);

		m_backgroundTexture.setSmooth(true);
		
		m_quitAction.addScancodeKeyControl(gf::Scancode::Escape);
		addAction(m_quitAction);

		m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
		addAction(m_fullscreenAction);

		m_clickButton.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
    	m_clickButton.setVolume(FxsVolume);
	}

	void CommonScene::playClickButton() {
		m_clickButton.setVolume(FxsVolume);
		m_clickButton.play();
	}

	void CommonScene::doHandleActions([[maybe_unused]] gf::Window& window) {    
		if (m_fullscreenAction.isActive()) {
			if(!m_game.connection->isActive()) {
				window.toggleFullscreen();
			}
		}

		if (m_quitAction.isActive()) {
			window.close();
		}
	}

	void CommonScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states) {
		gf::Coordinates coords(target);
		const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
		
		gf::Sprite background(m_backgroundTexture);
		if(!m_game.start->isActive()) { background.setColor(gf::Color::Opaque(0.20f)); }
		background.setScale(scale);
		target.draw(background, states);
	}
	
}