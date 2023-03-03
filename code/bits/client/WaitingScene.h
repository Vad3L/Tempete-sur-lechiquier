#ifndef TSL_WAITING_SCENE_H
#define TSL_WAITING_SCENE_H

#include <gf/Scene.h>
#include <gf/Font.h>
#include <SFML/Audio.hpp>

#include "../common/Network.h"
#include "GameModel.h"
#include "WaitingEntity.h"

namespace tsl {
	
	struct GameHub;

	class WaitingScene : public gf::Scene {
		public:
			WaitingScene(GameHub& game, Network& network);
		
		private:
			void doUpdate(gf::Time time) override;
	
			GameHub& m_game;
			Network& m_network;
			GameModel& m_model;

			WaitingEntity m_waiting;
	};

}

#endif // TSL_WAITING_SCENE_H