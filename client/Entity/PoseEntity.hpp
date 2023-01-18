#ifndef POSE_ENTITY_H
#define POSE_ENTITY_H

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

struct GameData;

class PoseEntity : public gf::Entity {
	public:
		PoseEntity(gf::ResourceManager& resources,GameData &gameData);
		
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states) override ;
		Card m_cardPose;
		
	private:
		gf::Texture& m_cardsIllustration;
		gf::Texture& m_accessories;
		gf::Font& m_poseNameFont;

		GameData &m_gameData;
};


#endif // POSE_ENTITY_H