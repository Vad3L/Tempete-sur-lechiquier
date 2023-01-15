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

struct GameData;

#include "../../model/Deck.hpp"

class CardsEntity : public gf::Entity {
    public:
        CardsEntity(gf::ResourceManager& resources,GameData &gameData);
        
        void update(gf::Time time) override;
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override ;

        int getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord);
    private:
        gf::Font& m_cardsFont;
        gf::Texture& m_cardsIllustration;



        GameData m_gameData;

};


#endif // CARDS_ENTITY_H