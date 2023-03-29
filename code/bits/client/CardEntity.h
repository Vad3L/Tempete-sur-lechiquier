#ifndef TSL_CARDS_ENTITY_H
#define TSL_CARDS_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/RenderTarget.h>

#include "GameModel.h"

namespace tsl {

    class CardEntity : public gf::Entity {
        public:
            CardEntity(gf::ResourceManager& resources, GameModel& model);
            
            void draw(gf::RenderTarget &target, const gf::RenderStates &states, int numC, gf::Vector2f boxCoord, float zoom=1.f,bool var=false, bool Scenezoom=false) ;

        private:

            GameModel& m_model;

            gf::Font& m_cardsFont;
            gf::Texture& m_cardsIllustration;
            gf::Texture& m_accessories;
    };
}

#endif // TSL_CARDS_ENTITY_H