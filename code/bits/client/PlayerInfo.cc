#include "PlayerInfo.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace tsl {
    
    namespace {
        static constexpr gf::Vector2i CardTextureSize = { 400, 500 };
        static constexpr float RelativeCardWidth = 0.15f;
        static constexpr float RelativeCardHeight = 0.45f;
        static constexpr int numberImageW = 10;
        static constexpr int numberImageH = 14;
    }
    
    PlayerInfo::PlayerInfo(gf::ResourceManager& resources, GameModel& model)
    : m_model(model)
    , m_font1(resources.getFont("fonts/RifficFree-Bold.ttf"))
    , m_font2(resources.getFont("fonts/DejaVuSans.ttf"))
    , m_cardsIllustration(resources.getTexture("images/CardsIllustrationSheet.png"))
    , m_accessories(resources.getTexture("images/AccesoriesCards.png"))
    , m_card(resources, m_model)
    {

    }

    int PlayerInfo::getSelectedCard([[maybe_unused]] const gf::Vector2f &screenSize, const gf::Vector2f& mouseCoords) {
        int cardSelected = -1;
        
        if(mouseCoords.y < -120){
            //gf::Log::debug("clique en dehors des cartes \n");
            return -1;
        }

        if(mouseCoords.x >= -600 && mouseCoords.x <= -400 ){
            cardSelected = 0;
        }
        if(mouseCoords.x >= -350 && mouseCoords.x <= -150){
            cardSelected = 1;
        }
        if(mouseCoords.x >= -100 && mouseCoords.x <= 100){
            cardSelected =  2;
        }
        if(mouseCoords.x >= 150 && mouseCoords.x <= 350){
            cardSelected  = 3;
        }
        if(mouseCoords.x >= 400 && mouseCoords.x <= 600){
            cardSelected = 4;
        }

        //gf::Log::debug("carte selectionnée %i\n",cardSelected);
        return cardSelected;
    }

    void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        constexpr float spaceBetweenCard = 0.208f;

        int cpt = 0;
        for (auto &c : m_model.cards) {
            float y = -0.4f;
            if(cpt==m_model.hoverCard) {
                y = -0.5f;
            }
            m_card.draw(target,states,c, gf::Vector2f(-0.5f+spaceBetweenCard*cpt, y));
            cpt++;
        }
    }

}