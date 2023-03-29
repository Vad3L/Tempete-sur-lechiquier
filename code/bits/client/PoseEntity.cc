#include "PoseEntity.h"

#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/Text.h>

namespace tsl {

    PoseEntity::PoseEntity(gf::ResourceManager& resources,GameModel &model)
    : m_cardPlaying(-1)
    , m_cardDiscard(-1)
    , m_model(model)
    , m_poseNameFont(resources.getFont("fonts/DejaVuSans.ttf"))
    , m_cardEntity(resources, m_model)
    {
        
    }

    void PoseEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){
        gf::Coordinates coords(target);
        const float min_scale = std::min((coords.getWindowSize() / gf::vec(1920.0f, 1080.0f)).x, (coords.getWindowSize() / gf::vec(1920.0f, 1080.0f)).y);
        gf::Vector2i sizeCard = gf::Vector2i((20)+200*min_scale,(120)+200*min_scale);

        gf::Vector2f position1 = coords.getRelativePoint({ 0.15f, 0.4f });
        gf::Vector2f position2 = coords.getRelativePoint({ 0.85f, 0.4f });
        
        gf::RoundedRectangleShape rect(sizeCard);
        rect.setColor(gf::Color::fromRgba32(0,0,0,0));
        rect.setOutlineThickness(6.f*min_scale);
        rect.setRadius(22*min_scale);
        rect.setPosition(position1);
        rect.setAnchor(gf::Anchor::Center);

        gf::Text poseName(m_model.getWord("Playing card"), m_poseNameFont, 16.f);
        poseName.setPosition({position1.x,position1.y-sizeCard.y/2.f-5.f});
        poseName.setAnchor(gf::Anchor::Center);
        poseName.setScale(min_scale);

        gf::Text poseHelp(m_model.getWord("A (remove) | Z (zoom)"), m_poseNameFont, 25.f);
        poseHelp.setPosition({position1.x,position1.y+sizeCard.y/1.5f});
        poseHelp.setAnchor(gf::Anchor::Center);
        poseHelp.setColor(gf::Color::White);
        poseHelp.setScale(min_scale);

        switch (m_model.gamePhase.getCurrentPhase()) {
            case Phase::AVANT_COUP :
                rect.setOutlineColor(gf::Color::Yellow);
                poseName.setColor(gf::Color::Yellow);
                break;
            case Phase::APRES_COUP :
                rect.setOutlineColor(gf::Color::Green);
                poseName.setColor(gf::Color::Green);
                break;
            case Phase::PAS_MON_TOUR :
                rect.setOutlineColor(gf::Color::fromRgba32(237,101,211));
                poseName.setColor(gf::Color::fromRgba32(237,101,211));
                break;
            default :
                rect.setOutlineColor(gf::Color::White);
                poseName.setColor(gf::Color::White);
                break;
        }
        if(m_model.chessStatus != ChessStatus::ON_GOING) {
            rect.setOutlineColor(gf::Color::White);
            poseName.setColor(gf::Color::White);
        }

        gf::RectangleShape placementTexte;
        placementTexte.setSize({145.f,20.0f});
        placementTexte.setScale(min_scale);
        placementTexte.setColor(gf::Color::Black);
        placementTexte.setPosition({position1.x,position1.y-sizeCard.y/2.f-2.5f});
        placementTexte.setAnchor(gf::Anchor::Center);

        target.draw(rect, states);
        target.draw(placementTexte,states);
        target.draw(poseName,states);
        target.draw(poseHelp,states);


        //coords.getRelativePoint({ 0.000125f, 0.0009f })
        if(m_cardPlaying !=- 1) {
            m_cardEntity.draw(target, states, m_cardPlaying, {position1.x-sizeCard.x/2+10, position1.y-sizeCard.y/2+10}, min_scale ,true);
        } 
    
        rect.setOutlineColor(gf::Color::White);
        rect.setPosition(position2);
        rect.setAnchor(gf::Anchor::Center);

        placementTexte.setSize({130.f,20.0f});
        placementTexte.setScale(min_scale);
        placementTexte.setPosition({position2.x,position2.y-sizeCard.y/2.f-2.5f});
        placementTexte.setAnchor(gf::Anchor::Center);

        poseName.setString(m_model.getWord("Discard"));
        poseName.setColor(gf::Color::White);
        poseName.setPosition({position2.x,position2.y-sizeCard.y/2.f-5.f});
        poseName.setAnchor(gf::Anchor::Center);


        poseHelp.setString(m_model.getWord("E (zoom)"));
        poseHelp.setColor(gf::Color::White);
        poseHelp.setPosition({position2.x,position2.y+sizeCard.y/1.5f});
        poseHelp.setAnchor(gf::Anchor::Center);


        target.draw(rect, states);
        target.draw(placementTexte,states);
        target.draw(poseName,states);
        target.draw(poseHelp,states);
        
        if(m_cardDiscard != -1) {
            m_cardEntity.draw(target, states, m_cardDiscard, {position1.x-sizeCard.x/2+10, position1.y-sizeCard.y/2+10}, min_scale ,true);
        }
    }

    void PoseEntity::returnCardHand() {
        if(m_cardPlaying == -1) {
            return;
        }

        for(auto &c : m_model.cards ) {
            if(c == -1) {
                c = m_cardPlaying;
                m_cardPlaying = -1;
                break;
            }
        }
    }

}