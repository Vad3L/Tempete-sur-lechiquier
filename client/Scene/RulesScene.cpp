#include "RulesScene.hpp"
#include "../GameHub.hpp"

#include "../Singletons.hpp"

RulesScene::RulesScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("Quit")
, m_downAction("DownAction")
, m_leftAction("PageLeft")
, m_rightAction("PageRight")
, m_fullscreenAction("FullScreen")
, m_triggerAction("TriggerAction")
, m_quitButton(game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButton.png"),game.resources.getTexture("images/button/menuButtonSelected.png"))
, m_pageLeft(game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrowSelected.png"))
, m_pageRight(game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrowSelected.png"))
, m_rulesEntity(game.resources)
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

	btnClicked.setBuffer(gAudioManager().getSound("sounds/button_click.ogg"));
	btnClicked.setVolume(FxsVolume);

	auto setupButton = [&] (gf::SpriteWidget& button, auto callback) {
		button.setAnchor(gf::Anchor::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_quitButton, [&] () {
		gf::Log::debug("Quit pressed!\n");
		btnClicked.play();
		m_game.replaceAllScenes(*m_game.menu);
	});

	setupButton(m_pageRight, [&] () {
		gf::Log::debug("Page up pressed!\n");
		btnClicked.play();
		changeRightLeft(true);
	});

	setupButton(m_pageLeft, [&] () {
		gf::Log::debug("Page down pressed!\n");
		btnClicked.play();
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
		m_game.replaceScene(*m_game.start);
	}

	if(m_leftAction.isActive()) {
		m_pageLeft.triggerCallback();
		while(m_pageLeft.isDefault()){
			m_widgets.selectPreviousWidget();
		}
	}

	if(m_rightAction.isActive()) {
		m_pageRight.triggerCallback();
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

	m_quitButton.setPosition(coords.getRelativePoint({0.5f, 0.825f}));
	
	m_pageLeft.setPosition(coords.getRelativePoint({( m_rulesEntity.m_index>0 ? 0.15f : -2.87f), 0.4}));

	
	m_pageRight.setPosition(coords.getRelativePoint({( m_rulesEntity.m_index<3 ? 0.85f : 2.87f), 0.4}));
	
	m_pageRight.setScale(1.f/2.f);
	m_pageLeft.setScale(1.f/2.f);
	m_quitButton.setScale(1.f/2.f);

	m_rulesEntity.render(target,states);
	m_widgets.render(target, states);
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