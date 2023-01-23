#ifndef CARDS_ENTITY_H
#define CARDS_ENTITY_H

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


class CardEntity : public gf::Entity {
	public:
		CardEntity(gf::ResourceManager& resources);
		
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states, Card c, float x, float y, int zoom=1) ;

	private:
		gf::Font& m_cardsFont;
		gf::Texture& m_cardsIllustration;
		gf::Texture& m_accessories;
};


#endif // CARDS_ENTITY_H