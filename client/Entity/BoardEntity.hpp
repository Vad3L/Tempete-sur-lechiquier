#ifndef BOARD_ENTITY_H
#define BOARD_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>

#include "../../model/Plateau.hpp"
#include "../../model/chess.hpp"
#include "../GameData.hpp"

struct GameData;

class BoardEntity : public gf::Entity {
    public:
        BoardEntity(gf::ResourceManager& resources, GameData &gameData);
        void update(gf::Time time) override;
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        gf::Vector2i getTransformCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);

    private:
        GameData &m_gameData;

        gf::Texture& m_backgroundTexture;

};




#endif // BOARD_ENTITY_H
