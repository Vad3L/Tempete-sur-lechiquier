#include "SettingsScene.hpp"

#include "../GameHub.hpp"

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
, m_backgroundTexture(game.resources.getTexture("images/startMenu1.png"))
, m_quitButton("Menu", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_rightButton(">", game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_leftButton("<", game.resources.getFont("fonts/DejaVuSans.ttf"))
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

	auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
		button.setDefaultTextColor(gf::Color::Black);
		button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
		button.setSelectedTextColor(gf::Color::Black);
		button.setSelectedBackgroundColor(gf::Color::fromRgba32(212,30,27,255));
		button.setDisabledTextColor(gf::Color::Black);
		button.setDisabledBackgroundColor(gf::Color::Red);
		button.setAnchor(gf::Anchor::TopLeft);
		button.setAlignment(gf::Alignment::Center);
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
	constexpr float characterSize = 0.075f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
	constexpr gf::Vector2f backgroundSizeArrow(0.2f, 0.3f);

	gf::Coordinates coords(target);

	float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

	const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paragraphWidthArrow = coords.getRelativeSize(backgroundSizeArrow - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);
	
	m_quitButton.setCharacterSize(resumeCharacterSize);
	m_quitButton.setPosition(coords.getRelativePoint({0.275f, 0.825f}));
	m_quitButton.setParagraphWidth(paragraphWidth);
	m_quitButton.setPadding(paddingSize);
	
	if(m_gameData.m_style > 0) {
		m_leftButton.setCharacterSize(resumeCharacterSize);
		m_leftButton.setPosition(coords.getRelativePoint({0.075f, 0.425f + characterSize }));
		m_leftButton.setParagraphWidth(paragraphWidthArrow);
		m_leftButton.setPadding(paddingSize);
	}else {
		m_leftButton.setPosition(coords.getRelativePoint({2.775, 0.425f + characterSize }));
	}

	if(m_gameData.m_style < 1) {
		m_rightButton.setCharacterSize(resumeCharacterSize);
		m_rightButton.setPosition(coords.getRelativePoint({0.775, 0.425f + characterSize }));
		m_rightButton.setParagraphWidth(paragraphWidthArrow);
		m_rightButton.setPadding(paddingSize);
	}else {
		m_rightButton.setPosition(coords.getRelativePoint({2.775, 0.425f + characterSize }));
	}

	m_widgets.render(target, states);
	m_settingsEntity.render(target, states);
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

