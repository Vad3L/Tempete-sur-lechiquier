#ifndef TSL_PLAYER_INFO_H
#define TSL_PLAYER_INFO_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "GameModel.h"
#include "CardEntity.h"

namespace tsl {

	class PlayerInfo: public gf::Entity {
		public:
			PlayerInfo(gf::ResourceManager& resources, GameModel &model);

			int getSelectedCard([[maybe_unused]] const gf::Vector2f &screenSize, const gf::Vector2f& mouseCoords);

			void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

		private:

			GameModel& m_model;

			gf::Font& m_font1;
			gf::Font& m_font2;

			gf::Texture& m_cardsIllustration;
			gf::Texture& m_accessories;
			CardEntity m_card;
	};

}


#endif // TSL_PLAYER_INFO_H