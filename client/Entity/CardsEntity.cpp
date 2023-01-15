#include "CardsEntity.hpp"




CardsEntity::CardsEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsFont(resources.getFont("fonts/DejaVuSans.ttf"))
,m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
,m_gameData(gameData)
{

}

void CardsEntity::update([[maybe_unused]] gf::Time time) {
}



void CardsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){
    // tmp
    int numberImageW = 10;
    int numberImageH = 14;
    gf::Coordinates coordsCard({1000,1000});
    gf::Vector2i sizeCard = gf::Vector2i(200,300);
    constexpr float titleCharacterSize = 20.f;
    constexpr float instructionsCharacterSize = 14.f;
    constexpr float spaceBetweenCard = 0.22f;

    int cpt = 0;

    for (auto &c : m_gameData.m_main) {
        
        if(c.m_num == -1) {
            continue;
        }
        
        int i = c.m_num%numberImageW;
        int j = c.m_num/numberImageW;       
        gf::Vector2f position = coordsCard.getRelativePoint({ -0.53f+spaceBetweenCard*cpt, 0.22f });

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
        cardName.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/1.9f});
        cardName.setAnchor(gf::Anchor::Center);

        gf::Text cardDescription(c.m_description.substr(0, 50), m_cardsFont, instructionsCharacterSize);
        cardDescription.setColor(gf::Color::Black);
        cardDescription.setPosition({position.x+10, position.y+sizeCard.y/1.5f});
        cardDescription.setParagraphWidth(200-20);
        cardDescription.setAlignment(gf::Alignment::Center);

        target.draw(card, states);
        target.draw(illustration, states);
        target.draw(cardName, states);
        target.draw(cardDescription, states);

        cpt++;
    }
}

int CardsEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    return 0;
}