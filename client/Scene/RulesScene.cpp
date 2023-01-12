#include "RulesScene.hpp"
#include "../GameHub.hpp"


RulesScene::RulesScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("Quit")
, m_triggerAction("TriggerAction")
, m_quitButton("Main Menu", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_pageLeft("<", game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_pageRight(">", game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_rulesEntity(game.resources)
, m_downAction("DownAction")
, m_leftAction("PageLeft")
, m_rightAction("PageRight")
, m_fullscreenAction("FullScreen")
{
	setClearColor(gf::Color::Black);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);

	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_downAction.addScancodeKeyControl(gf::Scancode::Down);
	addAction(m_downAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_triggerAction);

	m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
    addAction(m_leftAction);

    m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
    addAction(m_rightAction);

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

	setupButton(m_quitButton, [&] () {
		gf::Log::debug("Quit pressed!\n");
		m_game.replaceAllScenes(m_game.menu);
	});

	setupButton(m_pageRight, [&] () {
		gf::Log::debug("Page up pressed!\n");
		changeRightLeft(true);
	});

	setupButton(m_pageLeft, [&] () {
		gf::Log::debug("Page down pressed!\n");
		changeRightLeft(false);
	});

}

void RulesScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
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
		m_game.replaceScene(m_game.start);
	}

	if(m_leftAction.isActive()) {
		changeRightLeft(false);
		while(m_pageLeft.isDefault()){
            m_widgets.selectPreviousWidget();
        }
    }

    if(m_rightAction.isActive()) {
		changeRightLeft(true);
		while(m_pageRight.isDefault()){
            m_widgets.selectPreviousWidget();
        }
    }

}

void RulesScene::doProcessEvent(gf::Event& event) {
	switch (event.type)	{
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
		break;
	}
}

void RulesScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {

	target.setView(getHudView());
	gf::Coordinates coords(target);

	constexpr float characterSize = 0.075f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
	constexpr gf::Vector2f backgroundSizeArrow(0.1f, 0.3f);
	
	float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paragraphWidthArrow = coords.getRelativeSize(backgroundSizeArrow - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

	m_quitButton.setCharacterSize(resumeCharacterSize);
	m_quitButton.setPosition(coords.getRelativePoint({0.275f, 0.825f}));
	m_quitButton.setParagraphWidth(paragraphWidth);
	m_quitButton.setPadding(paddingSize);
	
	m_pageLeft.setCharacterSize(resumeCharacterSize);
	m_pageLeft.setParagraphWidth(paragraphWidthArrow);	
	m_pageLeft.setPadding(paddingSize);
	m_pageLeft.setAnchor(gf::Anchor::Center);
	if(m_rulesEntity.m_index>0) {
		m_pageLeft.setPosition(coords.getRelativePoint({0.15f, 0.4}));
	}else {
		m_pageLeft.setPosition(coords.getRelativePoint({-2.87f, 0.4}));
	}
	
	m_pageRight.setCharacterSize(resumeCharacterSize);
	m_pageRight.setParagraphWidth(paragraphWidthArrow);
	m_pageRight.setPadding(paddingSize);
	m_pageRight.setAnchor(gf::Anchor::Center);
	if(m_rulesEntity.m_index<3) {
		m_pageRight.setPosition(coords.getRelativePoint({0.85f, 0.4}));
	}else {
		m_pageRight.setPosition(coords.getRelativePoint({2.87f, 0.4}));
	}
	

	m_widgets.render(target, states);
	m_rulesEntity.render(target,states);
}

void RulesScene::doShow() {
	m_widgets.clear();

	m_quitButton.setDefault();
	m_pageLeft.setDefault();
	m_pageRight.setDefault();

	m_widgets.addWidget(m_quitButton);
	m_widgets.addWidget(m_pageLeft);
	m_widgets.addWidget(m_pageRight);
}

void RulesScene::changeRightLeft(bool value) {
	if(value){
        if(m_rulesEntity.m_index < 3){
			m_rulesEntity.m_index++;
        }
    }else{
        if(m_rulesEntity.m_index > 0){
            m_rulesEntity.m_index--;
        }
    }
}

void RulesScene::onActivityChange(bool active){
    if(active){
        m_rulesEntity.m_index = 0;
	}
}