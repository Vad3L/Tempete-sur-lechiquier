#include "ZoomCardScene.hpp"
#include "../GameHub.hpp"

ZoomCardScene::ZoomCardScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("Quit")
, m_fullscreenAction("FullScreen")
, m_cardEntity(game.resources)
, m_cardZoom()
{
	setClearColor(gf::Color::Black);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	m_quitAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_quitAction);
	
}

void ZoomCardScene::doProcessEvent(gf::Event& event) {
	
}

void ZoomCardScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_quitAction.isActive()) {
		m_game.popScene(); 
	}
}

void ZoomCardScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	if(m_cardZoom.m_num != 1) {
		m_cardEntity.render(target, states, m_cardZoom, 1.185f, 0.27f, 2); // 2 = zoom
	}
}
