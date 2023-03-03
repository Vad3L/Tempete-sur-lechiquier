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
    {

    }

    void PlayerInfo::updateSelectedCard(const gf::Vector2f &mouseCoords, const gf::Vector2f& screenSize) {
        if (m_model.selectedCard != -1) {
            return;
        }

        for (unsigned i = 0; i < m_model.cards.size(); ++i) {
            gf::RectF cardBounds = getCardBounds(screenSize, i);

            if (cardBounds.contains(mouseCoords)) {
                gf::Log::debug("(PlayerInfo) select card: %i\n", i);
                m_model.selectedCard = i;
                return;
            }
        }
    }

    void PlayerInfo::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        m_model.selectedCard =0;
        // Display cards
        for (unsigned i = 0; i < m_model.cards.size(); ++i) {
            int num = m_model.cards[i];
            if(num == -1) {
                continue;
            }

            auto bounds = getCardBounds(coords.getRelativeSize({ 1.0f, 1.0f }), i);
            auto cardSize = bounds.getSize();
            std::size_t w = m_model.deck[num].m_num%numberImageW;
	        std::size_t h = m_model.deck[num].m_num/numberImageW;	   

            float offsetSelected = 0.0f;
            if (m_model.selectedCard == static_cast<int>(i)) {
                offsetSelected = coords.getRelativeSize({ 0.0f, 0.06f }).height;
            }
            
            gf::RoundedRectangleShape card(cardSize);
            auto posCard = bounds.getBottomLeft();
            posCard.y -= offsetSelected;
            card.setRadius(coords.getRelativeCharacterSize(0.02f));
            card.setPosition(posCard);
            card.setAnchor(gf::Anchor::BottomLeft);
            
            gf::Vector2f sizeIllustration = { cardSize.x * 0.55f, cardSize.y * 0.35f };
            gf::Vector2f illustrationPos =  { posCard.x + cardSize.x * 0.5f, posCard.y - cardSize.y * 0.52f };
            gf::Vector2f cardDescriptionPos =  { posCard.x + cardSize.x * 0.5f, posCard.y - cardSize.y * 0.34f };
            float sizeDalto = std::min(cardSize.x, cardSize.y) * 0.1f;

            gf::Sprite illustration(m_cardsIllustration, gf::RectF::fromPositionSize({ (1.f / numberImageW) * w , (1.f / numberImageH) * h }, { 1.f / numberImageW, 1.f / numberImageH }));
            illustration.setScale(sizeIllustration / illustration.getLocalBounds().getSize());
            illustration.setPosition({ posCard.x + cardSize.x * 0.5f, posCard.y - cardSize.y * 0.96f });
            illustration.setAnchor(gf::Anchor::TopCenter);
            
            gf::Sprite back(m_accessories);
            back.setColor(gf::Color::Yellow);
            back.setScale(scale);
            back.setPosition(illustrationPos);
            back.setAnchor(gf::Anchor::Center);

            float scaleName = (m_model.deck[num].m_name.size() <= 20) ? 0.65f : 0.55f;
            gf::Text name(m_model.deck[num].m_name, m_font1, 30);
            name.setColor(gf::Color::Black);
            name.setParagraphWidth(back.getLocalBounds().getWidth() * 1.4f);
            name.setScale(scale * scaleName);
            name.setAlignment(gf::Alignment::Center);
            name.setPosition(illustrationPos);
            name.setAnchor(gf::Anchor::Center);
	        
            std::string description = m_model.deck[num].m_description.substr(0, 60) + "...";; 
            gf::Text cardDescription(description, m_font2, 25);
            cardDescription.setColor(gf::Color::Black);
            cardDescription.setParagraphWidth(back.getLocalBounds().getWidth() * 1.2f);
            cardDescription.setScale(scale * 0.8f);
            cardDescription.setAlignment(gf::Alignment::Center);
            cardDescription.setPosition(cardDescriptionPos);
            cardDescription.setAnchor(gf::Anchor::Center);
            
            gf::RoundedRectangleShape dalto({ sizeDalto, sizeDalto });
            
            switch (m_model.deck[num].m_turn) {
                case Turn::AVANT_COUP:
                    dalto.setColor(gf::Color::Yellow);
                    dalto.setRadius(50);
                    back.setColor(gf::Color::Yellow);
                    break;
                case Turn::APRES_COUP:
                    dalto.setColor(gf::Color::Green);
                    back.setColor(gf::Color::Green);
                    break;
                case Turn::BOTH:
                    dalto.setColor(gf::Color::fromRgba32(0,165,225));
                    dalto.setRotation(95);
                    back.setColor(gf::Color::fromRgba32(0,165,225));
                    break;
                case Turn::DURING_TOUR_ADVERSE:
                    dalto.setColor(gf::Color::fromRgba32(237,101,211));
                    dalto.setRadius(25);
                    back.setColor(gf::Color::fromRgba32(237,101,211));
                    break;
                default :
                    dalto.setColor(gf::Color::Black);
                    dalto.setRadius(25);
                    back.setColor(gf::Color::Black);
            }
            dalto.setPosition(bounds.getTopRight() - gf::Vector2f({ cardSize.x * 0.05f, offsetSelected - cardSize.y * 0.05f }));
            dalto.setAnchor(gf::Anchor::TopRight);

            target.draw(card, states);
            target.draw(illustration, states);
            target.draw(back, states);
            target.draw(name, states);
            target.draw(cardDescription, states);
            target.draw(dalto, states);
       }
    }

    gf::RectF PlayerInfo::getCardBounds(gf::Vector2f screenSize, std::size_t i) const {
        float newHeight = screenSize.height * RelativeCardHeight;
        float newWidth = screenSize.width * RelativeCardWidth;
        
        int pos = i - 2;
        float xPosition = 0.5f * screenSize.width + (pos) * (newWidth + 0.01f * screenSize.width) - newWidth * 0.5;
        float yPositon =  1.16f * screenSize.height - newHeight;
        return gf::RectF::fromPositionSize({xPosition, yPositon}, {newWidth, newHeight});
    }
}