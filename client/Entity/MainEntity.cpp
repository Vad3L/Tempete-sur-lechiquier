#include "MainEntity.hpp"

MainEntity::MainEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsFont(resources.getFont("fonts/DejaVuSans.ttf"))
,m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
,m_accessories(resources.getTexture("images/AccesoriesCards.png"))
,m_gameData(gameData)
{

}

void MainEntity::update([[maybe_unused]] gf::Time time) {
}

void MainEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){
    // tmp
    int numberImageW = 10;
    int numberImageH = 14;
    gf::Coordinates coordsCard({1200,300});
    gf::Vector2i sizeCard = gf::Vector2i(200,300);
    constexpr float titleCharacterSize = 16.f;
    constexpr float instructionsCharacterSize = 14.f;
    constexpr float spaceBetweenCard = 0.208f;

    int cpt = 0;
    for (auto &c : m_gameData.m_main) {
        
        if(c.m_num == -1) {
            continue;
        }
        
        int i = c.m_num%numberImageW;
        int j = c.m_num/numberImageW;       
        gf::Vector2f position = coordsCard.getRelativePoint({ -0.5f+spaceBetweenCard*cpt, -0.4f });
        gf::RoundedRectangleShape card(sizeCard);
        card.setColor(gf::Color::White);
        card.setRadius(22);
        card.setOutlineColor(gf::Color::Black);
        card.setPosition(position);

        gf::RectangleShape illustration({sizeCard.x/1.25f, sizeCard.x/1.25f});
        illustration.setTexture(m_cardsIllustration,gf::RectF::fromPositionSize({ (1.f / numberImageW) * i , (1.f/numberImageH) * j }, { 1.f / numberImageW, 1.f/numberImageH }));
        illustration.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/3.5f});
        illustration.setAnchor(gf::Anchor::Center);

        gf::Text cardName(c.m_name, m_cardsFont, titleCharacterSize);
        cardName.setColor(gf::Color::Black);
        cardName.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/1.58f});
        cardName.setAnchor(gf::Anchor::Center);

        gf::RectangleShape rect({sizeCard.x/1.1f, sizeCard.x/4.5f});
        rect.setTexture(m_accessories,gf::RectF::fromPositionSize({ 0.f  , 0.f }, { 1.f , 1.f }));
        rect.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/1.6f});
        rect.setAnchor(gf::Anchor::Center);
        switch (c.m_turn) {
            case Turn::BEFORE:
                rect.setColor(gf::Color::Chartreuse);
                break;
            case Turn::AFTER:
                rect.setColor(gf::Color::Yellow);
                break;
            case Turn::BOTH:
                rect.setColor(gf::Color::Green  );
                break;
            case Turn::DURING_TOUR_ADVERSE:
                rect.setColor(gf::Color::Blue);
                break;
        }

        gf::Text cardDescription(c.m_description.substr(0, 60)+"...", m_cardsFont, instructionsCharacterSize);
        cardDescription.setColor(gf::Color::Black);
        cardDescription.setPosition({position.x+10, position.y+sizeCard.y/1.3f});
        cardDescription.setParagraphWidth(200-20);
        cardDescription.setAlignment(gf::Alignment::Center);
         
        target.draw(card, states);
        target.draw(illustration, states);
        target.draw(rect, states);
        target.draw(cardName, states);
        target.draw(cardDescription, states);

        cpt++;
    }
}

int MainEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) { //sizeWindows = CardsView

    int cardSelected = -1;
    
    if(mouseCoord.y < -60){
        gf::Log::debug("clique en dehors des cartes \n");
        return -1;
    }
    if(mouseCoord.x >= -600 && mouseCoord.x <= -400 ){
        cardSelected = 0;
    }
    if(mouseCoord.x >= -360 && mouseCoord.x <= -160){
        cardSelected = 1;
    }
    if(mouseCoord.x >= -120 && mouseCoord.x <= 80){
        cardSelected =  2;
    }
    if(mouseCoord.x >= 120 && mouseCoord.x <= 320){
        cardSelected  = 3;
    }
    if(mouseCoord.x >= 360 && mouseCoord.x <= 560){
        cardSelected = 4;
    }

    gf::Log::debug("carte selectionnée %i\n",cardSelected);
    return cardSelected;

}