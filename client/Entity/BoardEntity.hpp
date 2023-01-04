#ifndef BOARD_ENTITY_H
#define BOARD_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>

#include "../../model/Plateau.hpp"
#include "../../model/chess.hpp"

class BoardEntity : public gf::Entity {
    public:
        BoardEntity(gf::ResourceManager& resources, Plateau &plateau);
        void update(gf::Time time) override;
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        gf::Vector2i getTransformCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);
        void setColor(ChessColor color);

    private:
        
        gf::Font& m_font;
        gf::Texture& m_backgroundTexture;

        float m_sizeSquare;
        gf::Vector2i m_caseSelected;
        ChessColor m_myColor;

        Plateau &m_plateau;
};




#endif // BOARD_ENTITY_H
