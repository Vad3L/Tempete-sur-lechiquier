#include "SettingsScene.hpp"

#include "../GameHub.hpp"
#include "../Singletons.hpp"
#include "../Singletons.hpp"

#include <fstream>

SettingsScene::SettingsScene(GameHub& game, GameData &gameData)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_gameData(gameData)
, m_leftAction("LeftAction")
, m_rightAction("RightAction")
, m_downAction("DownAction")
, m_upAction("UpAction")
, m_triggerAction("TriggerAction")
, m_fullscreenAction("FullscreenAction")
, m_quitAction("QuitAction")
, m_rightStyleButton(game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrowSelected.png"))
, m_leftStyleButton(game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrowSelected.png"))
, m_rightSoundButton(game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrowSelected.png"))
, m_leftSoundButton(game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrowSelected.png"))
, m_quitButton(game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButtonSelected.png"))
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

	m_upAction.addScancodeKeyControl(gf::Scancode::Up);
	addAction(m_upAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_triggerAction);

	btnClicked.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
	btnClicked.setVolume(FxsVolume);

	auto setupButton = [&] (gf::SpriteWidget& button, auto callback) {
		button.setAnchor(gf::Anchor::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_rightStyleButton, [&] () {
		gf::Log::debug("Next texture pressed!\n");
		btnClicked.play();
		changeTexture('+');
	});

	setupButton(m_leftStyleButton, [&] () {
		gf::Log::debug("Previous texture pressed!\n");
		btnClicked.play();
		changeTexture('-');
	});

	setupButton(m_rightSoundButton, [&] () {
		gf::Log::debug("Up sound pressed!\n");
		btnClicked.play();
		changeSound('+');
	});

	setupButton(m_leftSoundButton, [&] () {
		gf::Log::debug("Down sound pressed!\n");
		btnClicked.play();
		changeSound('-');
	});

	setupButton(m_quitButton, [&] () {
		gf::Log::debug("Quit settings pressed!\n");
		btnClicked.play();
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
		if(m_ligne==2) {return;}

		if(m_ligne==0) {
			while(m_rightStyleButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}else {
			while(m_rightSoundButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}
		m_widgets.triggerAction();
	}

	if (m_leftAction.isActive()) {
		if(m_ligne==2) {return;}

		if(m_ligne==0) {
			while(m_leftStyleButton.isDefault()){
				m_widgets.selectNextWidget();
			}
		}else {
			while(m_leftSoundButton.isDefault()){
				m_widgets.selectNextWidget();
			}
		}
		
		m_widgets.triggerAction();
	}

	if (m_downAction.isActive()) {
		if(m_ligne==2) {
			m_ligne=-1;
			m_settingsEntity.m_ligne=-1;
		}

		m_ligne+=1;
		m_settingsEntity.m_ligne +=1;

		if(m_ligne==0) {
			while(m_rightStyleButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}else if(m_ligne==1) {
			while(m_rightSoundButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}else if(m_ligne==2) {
			while(m_quitButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}
	}

	if (m_upAction.isActive()) {
		if(m_ligne==0) {
			m_ligne=3;
		 	m_settingsEntity.m_ligne=3;
		}

		m_ligne-=1;
		m_settingsEntity.m_ligne -=1;

		if(m_ligne==0) {
			while(m_rightStyleButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}else if(m_ligne==1) {
			while(m_rightSoundButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
		}else if(m_ligne==2) {
			while(m_quitButton.isDefault()) {
				m_widgets.selectPreviousWidget();
			}
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
		default :
			break;
	}
}

void SettingsScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {

	target.setView(getHudView());
	gf::Coordinates coords(target);
	
	m_quitButton.setPosition(coords.getRelativePoint({0.5f, 0.825f}));
	m_quitButton.setScale(1.f/2.f);
	
	m_leftStyleButton.setPosition(coords.getRelativePoint({(m_gameData.m_style > 0 ? 0.225f : 2.775f), 0.4f }));
	m_leftStyleButton.setScale(1.f/2.f);
	
	m_rightStyleButton.setScale(1.f/2.f);
	m_rightStyleButton.setPosition(coords.getRelativePoint({(m_gameData.m_style < 1 ? 0.775f : 2.775f), 0.4f }));

	m_leftSoundButton.setPosition(coords.getRelativePoint({(BackgroundAmbiantVolume > 0 ? 0.225f : 2.775f), 0.65f }));
	m_leftSoundButton.setScale(1.f/2.f);
	
	m_rightSoundButton.setScale(1.f/2.f);
	m_rightSoundButton.setPosition(coords.getRelativePoint({(BackgroundAmbiantVolume < 100 ? 0.775f : 2.775f), 0.65f }));
	
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

void SettingsScene::changeSound(char c) {
	if(c == '+') {
		if(BackgroundAmbiantVolume < 100) {
			BackgroundAmbiantVolume++;
		}
	}else {
		if(BackgroundAmbiantVolume > 0) {
			BackgroundAmbiantVolume--;
		}	
	}
	gBackgroundMusic.setVolume(BackgroundAmbiantVolume);
	gf::Log::error("change son jeu à %f\n", BackgroundAmbiantVolume);
	FxsVolume = (BackgroundAmbiantVolume == 0.f) ? 0.f : 74.f;
	btnClicked.setVolume(FxsVolume);
}

void SettingsScene::doShow() {
	m_widgets.clear();

	m_leftStyleButton.setDefault();
	m_rightStyleButton.setDefault();
	m_leftSoundButton.setDefault();
	m_rightSoundButton.setDefault();
	m_quitButton.setDefault();

	m_widgets.addWidget(m_quitButton);
	m_widgets.addWidget(m_rightStyleButton);
	m_widgets.addWidget(m_leftStyleButton);
	m_widgets.addWidget(m_rightSoundButton);
	m_widgets.addWidget(m_leftSoundButton);
	
}

void SettingsScene::onActivityChange(bool active){
	if(!active){	
		gf::Log::debug("Scene settings desactive\n");
		
		std::ofstream file(std::string(GAME_CONFIGDIR)+"Settings.txt");

		if(!file) {
			gf::Log::error("Impossible d'ouvrir le fichier.\n");
		}
		else {
			
			file << m_gameData.m_style << "\n";
			file << BackgroundAmbiantVolume;
			file.close();
		}
	}else {
		m_ligne = 0;
		m_settingsEntity.m_ligne = 0;
	}
}