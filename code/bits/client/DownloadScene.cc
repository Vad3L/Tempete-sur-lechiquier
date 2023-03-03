#include "DownloadScene.h"

#include "GameHub.h"

namespace tsl {

	DownloadScene::DownloadScene(GameHub& game)
	: gf::Scene(game.getRenderer().getSize())
	, m_game(game)
	, m_downloadEntity(m_game.resources, m_game.m_model) 
	{
		setClearColor(gf::Color::Black);

		addHudEntity(m_downloadEntity);
	}

	void DownloadScene::changeFrame() {
        m_downloadEntity.changeFrame();
    }

	void DownloadScene::doUpdate([[maybe_unused]] gf::Time time) {
		if (!isActive()) {
			return;
		}
		
		if (m_game.loadingFinished()) {
			gf::Ref<gf::Scene> scenes[] = { *m_game.common, *m_game.start };
			m_game.replaceScene(scenes, m_game.blackoutEffect, gf::seconds(2.0f));
		}
	}

}