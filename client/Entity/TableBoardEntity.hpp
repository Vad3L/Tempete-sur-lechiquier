#ifndef TABLE_BOARD_ENTITY_H
#define TABLE_BOARD_ENTITY_H

#include<iostream>

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include "../../model/Plateau.hpp"
#include "../../model/Chess.hpp"
#include "../GameData.hpp"

struct GameData;

class TableBoardEntity: public gf::Entity {
	public:
		TableBoardEntity(gf::ResourceManager& resources, GameData &gameData);
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
		
	private:

		gf::Font& m_font;
		gf::Texture& m_backgroundTexture;
		gf::Texture& m_backgroundTexture2;

		GameData &m_gameData;
};




#endif // TABLE_BOARD_ENTITY_H
