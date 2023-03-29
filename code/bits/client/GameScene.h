#ifndef TSL_GAME_SCENE_H
#define TSL_GAME_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "PlayerInfo.h"
#include "ButtonsGame.h"
#include "PoseEntity.h"
#include "BoardEntity.h"
#include "TableBoardEntity.h"
#include "../common/Network.h"

namespace tsl {

	struct GameHub;

	class GameScene : public gf::Scene {
		public:
			GameScene(GameHub &scenes, Network &network);

		private:
			void doHandleActions(gf::Window& window) override;
			void doProcessEvent(gf::Event& event) override;
			void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
			void doUpdate([[maybe_unused]] gf::Time time) override;
			void onActivityChange(bool active)  override;
			void doShow() override;
			
			GameHub &m_game;
			Network &m_network;
			GameModel &m_model;
			
			gf::ExtendView m_boardView;
			gf::ExtendView m_tableBoardView;
			gf::ExtendView m_cardsView;
			gf::ViewContainer m_views;
			
			ButtonChoice m_choice;
			ButtonsGame m_buttons;
			PlayerInfo m_info;

			PoseEntity m_poseEntity;
			BoardEntity m_boardEntity;
			TableBoardEntity m_tableBoardEntity;

			gf::Action m_poseActionRight;
			gf::Action m_poseActionLeft;
			gf::Action m_discardActionRight;
			gf::Action m_quitAction;

			bool m_poseRight;
			bool m_poseLeft;
			bool m_discardRight;

			sf::Sound clickEndTurn;
			sf::Sound clickplayCard;
			
			sf::Sound takePiece;
			sf::Sound movePiece;
			sf::Sound checkPiece;
			sf::Sound drawCard;

	};

}

#endif // TSL_GAME_SCENE_H
