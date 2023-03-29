#ifndef TSL_TABLE_BOARD_ENTITY_H
#define TSL_TABLE_BOARD_ENTITY_H

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

#include "GameModel.h"

namespace tsl {

    class TableBoardEntity: public gf::Entity {
        public:
            TableBoardEntity(gf::ResourceManager& resources, GameModel &model);
            void update(gf::Time time) override;
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
            
        private:
            GameModel &m_model;

            gf::Font &m_font;

            gf::Texture &m_backgroundTexture;
            gf::Texture &m_backgroundTexture2;
            gf::Texture &m_backgroundTexture3;
            
    };
}

#endif // TSL_TABLE_BOARD_ENTITY_H