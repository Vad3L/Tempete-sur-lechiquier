#ifndef TSL_GAME_SCENE_H
#define TSL_GAME_SCENE_H

#include <gf/Scene.h>

#include "PlayerInfo.h"
#include "../common/Network.h"

namespace tsl {

	struct GameHub;

	class GameScene : public gf::Scene {
		public:
			GameScene(GameHub& scenes, Network& network);

		private:
			GameHub& m_game;
			GameModel& m_model;

			PlayerInfo m_info;
	};

}

#endif // TSL_GAME_SCENE_H
