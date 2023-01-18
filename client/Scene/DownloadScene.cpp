#include "DownloadScene.hpp"

DownloadScene::DownloadScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_escapeAction("Escape")
{
	setClearColor(gf::Color::Red);
}

void DownloadScene::loadAnimation() {

}

void DownloadScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (m_escapeAction.isActive()) {
		m_game.popScene();
	}
}

void DownloadScene::doUpdate([[maybe_unused]] gf::Time time) {
if (!isActive()) {
	return;
}

	if (m_game.loadingFinish()) {
		m_game.replaceScene(*m_game.start, m_game.blackoutEffect, gf::seconds(2.0f));
		return;
	}
}
