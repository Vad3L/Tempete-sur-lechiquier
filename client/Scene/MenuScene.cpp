#include "MenuScene.hpp"
#include "../GameHub.hpp"


MenuScene::MenuScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_backgroundTexture(game.resources.getTexture("images/startMenu1.png"))
, m_upAction("UpAction")
, m_downAction("DownAction")
, m_triggerAction("TriggerAction")
, m_quitAction("Quit")
, m_fullscreenAction("Fullscreen")
, m_play("Jouer", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_rules("Regles", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_quit("Quitter", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
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

	setupButton(m_play, [&] () {
		gf::Log::debug("Play pressed!\n");
		m_game.replaceAllScenes(m_game.play);
	});

	setupButton(m_rules, [&] () {
		gf::Log::debug("Rules pressed!\n");
		m_game.replaceAllScenes(m_game.rules);
	});

	setupButton(m_quit, [&] () {
		gf::Log::debug("Quit pressed!\n");
		m_game.popAllScenes();
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


	target.setView(getHudView());

	unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);

	gf::Text title("Tempete sur l echiquier", m_game.resources.getFont("../data/fonts/Trajan-Color-Concept.otf"), titleCharacterSize);
	title.setColor(gf::Color::White);
	title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
	title.setAnchor(gf::Anchor::TopCenter);
	target.draw(title, states);

	constexpr float characterSize = 0.075f;
	constexpr float spaceBetweenButton = 0.045f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);

	const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

	m_play.setCharacterSize(resumeCharacterSize);
	m_play.setPosition(coords.getRelativePoint({0.275f, 0.41f}));
	m_play.setParagraphWidth(paragraphWidth);
	m_play.setPadding(paddingSize);

	m_rules.setCharacterSize(resumeCharacterSize);
	m_rules.setPosition(coords.getRelativePoint({0.275f, 0.425f + characterSize + spaceBetweenButton}));
	m_rules.setParagraphWidth(paragraphWidth);
	m_rules.setPadding(paddingSize);

	m_quit.setCharacterSize(resumeCharacterSize);
	m_quit.setPosition(coords.getRelativePoint({0.275f, 0.425f + (characterSize + spaceBetweenButton) * 2}));
	m_quit.setParagraphWidth(paragraphWidth);
	m_quit.setPadding(paddingSize);

	m_widgets.render(target, states);

	gf::Sprite background(m_backgroundTexture);
	background.setColor(gf::Color::Opaque(0.20f));
	background.setPosition(coords.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);
	target.draw(background, states);
}

void MenuScene::doShow() {
	m_widgets.clear();

	m_play.setDefault();
	m_widgets.addWidget(m_play);

	m_rules.setDefault();
	m_widgets.addWidget(m_rules);

	m_quit.setDefault();
	m_widgets.addWidget(m_quit);

	m_widgets.selectNextWidget();
}



