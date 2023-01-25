#include "SettingsScene.hpp"

#include "../GameHub.hpp"

#include <fstream>

SettingsScene::SettingsScene(GameHub& game, GameData &gameData)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_gameData(gameData)
, m_rightAction("RightAction")
, m_leftAction("LeftAction")
, m_downAction("DownAction")
, m_triggerAction("TriggerAction")
, m_quitAction("QuitAction")
, m_fullscreenAction("FullscreenAction")
, m_quitButton(game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButtonSelected.png"))
, m_leftButton(game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrowSelected.png"))
, m_rightButton(game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrowSelected.png"))
, m_settingsEntity(game.resources, m_gameData)
{
	setClearColor(gf::Color::Black);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
	addAction(m_rightAction);

	m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
	addAction(m_leftAction);

	m_downAction.addScancodeKeyControl(gf::Scancode::Down);
	addAction(m_downAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_triggerAction);

	auto setupButton = [&] (gf::SpriteWidget& button, auto callback) {
		button.setAnchor(gf::Anchor::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_rightButton, [&] () {
		gf::Log::debug("Next texture pressed!\n");
		changeTexture('+');
	});

	setupButton(m_leftButton, [&] () {
		gf::Log::debug("Previous texture pressed!\n");
		changeTexture('-');
	});

	setupButton(m_quitButton, [&] () {
		gf::Log::debug("Quit seetings pressed!\n");
		m_game.replaceAllScenes(*m_game.menu);
	});
}

void SettingsScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_rightAction.isActive()) {
		m_rightButton.triggerCallback();
		while(m_rightButton.isDefault()){
			m_widgets.selectNextWidget();
		}
	}

	if (m_leftAction.isActive()) {
		m_leftButton.triggerCallback();
		while(m_leftButton.isDefault()){
			m_widgets.selectPreviousWidget();
		}
	}

	if (m_downAction.isActive()) {
		while(m_quitButton.isDefault()){
			m_widgets.selectPreviousWidget();
		}
	}
	
	if (m_triggerAction.isActive()) {
		m_widgets.triggerAction();
	}

	if (m_quitAction.isActive()) {
		m_game.replaceAllScenes(*m_game.menu);
	}
}

void SettingsScene::doProcessEvent(gf::Event& event) {
	switch (event.type) {
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;
	}
}

void SettingsScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
	constexpr gf::Vector2f backgroundSizeArrow(0.2f, 0.3f);

	gf::Coordinates coords(target);
	
	m_quitButton.setPosition(coords.getRelativePoint({0.5f, 0.825f}));
	m_quitButton.setScale(1.f/2.f);
	
	m_leftButton.setPosition(coords.getRelativePoint({(m_gameData.m_style > 0 ? 0.225f : 2.775f), 0.425f }));
	m_leftButton.setScale(1.f/2.f);
	
	m_rightButton.setScale(1.f/2.f);
	m_rightButton.setPosition(coords.getRelativePoint({(m_gameData.m_style < 1 ? 0.775f : 2.775f), 0.425f }));

	
	m_settingsEntity.render(target, states);
	m_widgets.render(target, states);
}

void SettingsScene::changeTexture(char c) {
	if(c == '+') {
		if(m_gameData.m_style < 1) {
			m_gameData.m_style++;
		}
	}else {
		if(m_gameData.m_style > 0) {
			m_gameData.m_style--;
		}	
	}
}

void SettingsScene::doShow() {
	m_widgets.clear();

	m_leftButton.setDefault();
	m_rightButton.setDefault();
	m_quitButton.setDefault();

	m_widgets.addWidget(m_quitButton);
	m_widgets.addWidget(m_rightButton);
	m_widgets.addWidget(m_leftButton);
	
}

void SettingsScene::onActivityChange(bool active){
	if(!active){	
		gf::Log::debug("Scene settings desactive\n");
		
		std::ofstream file(std::string(GAME_CONFIGDIR)+"Settings.txt");

		if(!file) {
			gf::Log::error("Impossible d'ouvrir le fichier.\n");
		}
		else {
			
			file << m_gameData.m_style;
			file.close();
		}
	}
}