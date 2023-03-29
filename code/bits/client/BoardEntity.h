#ifndef TSL_BOARD_ENTITY_H
#define TSL_BOARD_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/RenderTarget.h>

#include "GameModel.h"

namespace tsl {

    class BoardEntity : public gf::Entity {
        public:
            BoardEntity(gf::ResourceManager& resources, GameModel &modlel);
            void update(gf::Time time) override;
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

            gf::Vector2i getCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);
            ChessPiece getChoice([[maybe_unused]] gf::Vector2i sizeWindows,gf::Vector2i clickCoord);

        private:
            GameModel &m_model;

            gf::Texture &m_backgroundTexture1;
            gf::Texture &m_backgroundTexture2;
            gf::Texture &m_backgroundTexture3;

            gf::Texture &m_target;
    };
}

#endif // TSL_BOARD_ENTITY_H