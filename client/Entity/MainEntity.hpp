#ifndef MAIN_ENTITY_H
#define MAIN_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include <vector>
#include <iostream>

#include "../GameData.hpp"
#include "../../model/Deck.hpp"
#include "./CardsEntity.hpp"

struct GameData;

class MainEntity : public gf::Entity {
	public:
		MainEntity(gf::ResourceManager& resources,GameData &gameData);
		
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states) override ;

		int getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);
	private:

		GameData &m_gameData;
		CardsEntity m_card;
};


#endif // MAIN_ENTITY_H