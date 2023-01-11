#ifndef TABLE_BOARD_ENTITY_H
#define TABLE_BOARD_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>

#include "../../model/Plateau.hpp"
#include "../../model/chess.hpp"
#include "../GameData.hpp"

struct GameDate;

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
