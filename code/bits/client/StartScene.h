#ifndef TSL_START_SCENE_H
#define TSL_START_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "StartEntity.h"

namespace tsl {
	
	struct GameHub;

	class StartScene : public gf::Scene {
		public:
			StartScene(GameHub& game);
			void doHandleActions(gf::Window& window) override;

			GameHub& m_game;
			
			gf::Action m_startAction;

			StartEntity m_startEntity;
	};

}

#endif // TSL_START_SCENE_H