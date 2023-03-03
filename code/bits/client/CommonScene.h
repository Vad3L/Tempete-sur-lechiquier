#ifndef TSL_COMMON_SCENE_H
#define TSL_COMMON_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "GameModel.h"

namespace tsl {

	struct GameHub;

	class CommonScene : public gf::Scene {
		public:
			CommonScene(GameHub& game);
			
			void playClickButton();

		private:
			void doHandleActions(gf::Window& window) override;
			void doProcessEvent(gf::Event& event) override;
			void doUpdate(gf::Time time) override;
			void doRender(gf::RenderTarget& target, const gf::RenderStates& states) override;

			GameHub& m_game;
			GameModel& m_model;
			
			gf::Action m_quitAction;
			gf::Action m_fullscreenAction;

			gf::Texture& m_backgroundTexture;
	};

}

#endif // TSL_COMMON_SCENE_H