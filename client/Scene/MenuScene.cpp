#include "MenuScene.hpp"
#include "../GameHub.hpp"


MenuScene::MenuScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_backgroundTexture(game.resources.getTexture("images/StartMenu.png"))
, m_upAction("UpAction")
, m_downAction("DownAction")
, m_triggerAction("TriggerAction")
, m_quitAction("Quit")
, m_fullscreenAction("Fullscreen")
, m_play(game.resources.getTexture("images/button/JouerButton.png"),game.resources.getTexture("images/button/JouerButton.png"),game.resources.getTexture("images/button/JouerButtonSelected.png"))
, m_rules(game.resources.getTexture("images/button/reglesButton.png"),game.resources.getTexture("images/button/reglesButton.png"),game.resources.getTexture("images/button/reglesButtonSelected.png"))
, m_quit(game.resources.getTexture("images/button/quitterButton.png"),game.resources.getTexture("images/button/quitterButton.png"),game.resources.getTexture("images/button/quitterButtonSelected.png"))
, m_settings(game.resources.getTexture("images/Cog.png"),game.resources.getTexture("images/Cog.png"),game.resources.getTexture("images/Cog.png"))
{
	setClearColor(gf::Color::Black);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);

	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_upAction.addScancodeKeyControl(gf::Scancode::Up);
	addAction(m_upAction);

	m_downAction.addScancodeKeyControl(gf::Scancode::Down);
	addAction(m_downAction);

	m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_triggerAction);

	auto setupButtonSprite = [&] (gf::SpriteWidget& button, auto callback) {
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	m_cog = 0;
	m_settings.setScale(1.f/5.f);

	setupButtonSprite(m_play, [&] () {
		gf::Log::debug("Play pressed!\n");
		m_game.replaceAllScenes(*m_game.play);
	});

	setupButtonSprite(m_rules, [&] () {
		gf::Log::debug("Rules pressed!\n");
		m_game.replaceAllScenes(*m_game.rules);
	});

	setupButtonSprite(m_quit, [&] () {
		gf::Log::debug("Quit pressed!\n");
		m_game.popAllScenes();
	});

	setupButtonSprite(m_settings, [&] () {
		gf::Log::debug("Settings pressed!\n");
		m_game.replaceAllScenes(*m_game.settings);
	});	
}

void MenuScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_upAction.isActive()) {
		m_widgets.selectPreviousWidget();
	}

	if (m_downAction.isActive()) {
		m_widgets.selectNextWidget();
	}

	if (m_triggerAction.isActive()) {
		m_widgets.triggerAction();
	}

	if (m_quitAction.isActive()) {
		m_game.popAllScenes();
	}
}

void MenuScene::doProcessEvent(gf::Event& event) {
	switch (event.type) {
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;
	}
}

void MenuScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	gf::Coordinates coords(target);

	float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

	gf::Sprite background(m_backgroundTexture);
	background.setColor(gf::Color::Opaque(0.20f));
	background.setPosition(coords.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);

	target.setView(getHudView());

	unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);

	gf::Text title("Tempete sur l echiquier", m_game.resources.getFont("fonts/Trajan-Color-Concept.otf"), titleCharacterSize);
	title.setColor(gf::Color::White);
	title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
	title.setAnchor(gf::Anchor::TopCenter);
	target.draw(title, states);

	constexpr float spaceBetweenButton = 0.2f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
	
	m_play.setPosition(coords.getRelativePoint({0.5f, 0.41f}));
	m_play.setScale(1.f/2.f);
	m_play.setAnchor(gf::Anchor::Center);

	m_rules.setPosition(coords.getRelativePoint({0.5f, 0.418f + spaceBetweenButton}));
	m_rules.setScale(1.f/2.f);
	m_rules.setAnchor(gf::Anchor::Center);

	m_quit.setPosition(coords.getRelativePoint({0.5f, 0.425f + spaceBetweenButton * 2}));
	m_quit.setScale(1.f/2.f);
	m_quit.setAnchor(gf::Anchor::Center);

	m_settings.setPosition(coords.getRelativePoint({0.1f, 0.1f}));
	m_settings.setAnchor(gf::Anchor::Center);
	
	if(m_settings.isSelected()){(m_cog < gf::Pi*2) ? m_cog+= 0.01 : m_cog=0 ;}
	m_settings.setRotation(m_cog);
	m_settings.setPosition(coords.getRelativePoint({0.9f, 0.9f }));
	
	target.draw(background, states);
	m_widgets.render(target, states);	
}

void MenuScene::doShow() {
	m_widgets.clear();

	m_play.setDefault();
	m_widgets.addWidget(m_play);

	m_rules.setDefault();
	m_widgets.addWidget(m_rules);

	m_quit.setDefault();
	m_widgets.addWidget(m_quit);

	m_settings.setDefault();
	m_widgets.addWidget(m_settings);

	m_widgets.selectNextWidget();
}



