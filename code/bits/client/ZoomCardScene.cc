#include "ZoomCardScene.h"
#include "GameHub.h"

namespace tsl {
        
    ZoomCardScene::ZoomCardScene(GameHub& game)
    : gf::Scene(game.getRenderer().getSize())
    , m_numCard(-1)
    , m_game(game)
    , m_quitAction("Quit")
    , m_cardsFont(m_game.resources.getFont("fonts/DejaVuSans.ttf"))
    , m_rulesFont(m_game.resources.getFont("fonts/RifficFree-Bold.ttf"))
    , m_cardEntity(m_game.resources, m_game.m_model)
    {
        setClearColor(gf::Color::Black);
        
        m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
        m_quitAction.addMouseButtonControl(gf::MouseButton::Left);
        addAction(m_quitAction);

        m_principalView = gf::ExtendView({ 0.f, 0.f }, {(float)game.getRenderer().getSize().x, (float)game.getRenderer().getSize().y });
        m_principalView.setViewport(gf::RectF::fromPositionSize({ 0.f, 0.f}, { 1.f, 1.f }));
        m_principalView.setCenter(game.getRenderer().getSize()/2);

        m_views.addView(m_principalView);
        m_principalView.onFramebufferSizeChange(game.getRenderer().getSize());
        m_views.setInitialFramebufferSize({game.getRenderer().getSize()});
        
    }

    void ZoomCardScene::doProcessEvent([[maybe_unused]] gf::Event& event) {
        m_views.processEvent(event);
    }

    void ZoomCardScene::doHandleActions([[maybe_unused]] gf::Window& window) {
        if (!isActive()) {
            return;
        }
        if (m_quitAction.isActive()) {
            m_game.popScene(); 
        }
    }

    void ZoomCardScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        target.setView(getHudView());
        gf::RoundedRectangleShape background(target.getSize());
        background.setColor(gf::Color::fromRgba32(0,0,0,200));
        target.draw(background, states);

        target.setView(m_principalView);

        if(m_numCard != -1) {
            gf::Coordinates coords({1920,1080});

            unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.04f);
            
            gf::Text bubbleTittle(m_game.m_model.getWord("Card Description ") + m_game.m_model.deck[m_numCard].m_name, m_rulesFont, 35);	
            bubbleTittle.setColor(gf::Color::Yellow);
            bubbleTittle.setPosition(coords.getRelativePoint({ 0.65f, 0.27f}));
            bubbleTittle.setAnchor(gf::Anchor::TopCenter);
            bubbleTittle.setOutlineColor(gf::Color::Black);
            bubbleTittle.setOutlineThickness(titleCharacterSize/6.f);


            gf::RoundedRectangleShape infoBubble(gf::Vector2i({1100,600}));
            infoBubble.setColor(gf::Color::White);
            infoBubble.setRadius(22);
            infoBubble.setOutlineColor(gf::Color::Black);
            infoBubble.setPosition(coords.getRelativePoint({0.35f,0.21f}));
            //infoBubble.setAnchor(gf::Anchor::Center);
            
            gf::Text cardDescription(m_game.m_model.deck[m_numCard].m_description[m_game.m_model.language] , m_rulesFont, 25);
        
            cardDescription.setColor(gf::Color::Black);
            cardDescription.setPosition(coords.getRelativePoint({0.64f , 0.5f}));
            cardDescription.setParagraphWidth(infoBubble.getLocalBounds().getWidth()-250);
            cardDescription.setAlignment(gf::Alignment::Center);
            cardDescription.setAnchor(gf::Anchor::Center);

            std::string timeToPlay;
            switch(m_game.m_model.deck[m_numCard].m_turn){
                case Turn::AVANT_COUP:
                    timeToPlay = m_game.m_model.getWord("Play this card before your move");
                    break;
                case Turn::APRES_COUP:
                    timeToPlay = m_game.m_model.getWord("Play this card after your move"); 
                    break;
                case Turn::BOTH:
                    timeToPlay = m_game.m_model.getWord("Play this card before or after your move");
                    break;
                case Turn::PDT_TOUR_ADVERSE:
                    timeToPlay = m_game.m_model.getWord("Play this card during the opponent's move");
                    break;
                default :
                    timeToPlay = m_game.m_model.getWord("This card is not implem");
            }

            gf::Text cardTimeToPlay(timeToPlay, m_rulesFont, 25);
        
            cardTimeToPlay.setColor(gf::Color::Black);
            cardTimeToPlay.setPosition(coords.getRelativePoint({0.64f, 0.72f}));
            cardTimeToPlay.setParagraphWidth(infoBubble.getLocalBounds().getWidth()-250);
            cardTimeToPlay.setAlignment(gf::Alignment::Center);
            cardTimeToPlay.setAnchor(gf::Anchor::Center);

            m_cardEntity.draw(target, states, m_numCard, gf::Vector2f(0.15f, 0.75f), 2, false, true); // 2 = zoom
            target.draw(infoBubble, states);
            target.draw(bubbleTittle,states);
            target.draw(cardDescription,states);
            target.draw(cardTimeToPlay,states);

        }
    }

    void ZoomCardScene::onActivityChange(bool active) {
        if(active) {
            gf::Log::debug("scene zoom active\n");
            m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());
        }
    }

}