#ifndef BOARD_ENTITY_H
#define BOARD_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include<iostream>

#include "../../model/Plateau.hpp"
#include "../../model/Chess.hpp"
#include "../GameData.hpp"

struct GameData;

class BoardEntity : public gf::Entity {
	public:
		BoardEntity(gf::ResourceManager& resources, GameData &gameData);
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

		gf::Vector2i getCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);
		ChessPiece getChoice(gf::Vector2i sizeWindows,gf::Vector2i clickCoord);

	private:
		
		gf::Texture& m_backgroundTexture;
		gf::Texture& m_backgroundTexture2;

		GameData &m_gameData;
};




#endif // BOARD_ENTITY_H
