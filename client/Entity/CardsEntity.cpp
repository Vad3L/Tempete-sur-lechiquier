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
    
    gf::Vector2i sizeCard = gf::Vector2i(200,300);
    constexpr float instructionsCharacterSize = 0.075f;
    constexpr float spaceBetweenCard = 5.f;

    for (Card &c : m_gameData.m_main) {
        int i = c.getNum()%numberImageW;
        int j = c.getNum()/numberImageW;

        gf::Coordinates coordsCard({1000,1000});
        gf::Vector2f position = coordsCard.getRelativePoint({ -0.4f, 0.3f });

        gf::RectangleShape illustration1({100.f,100.f});
        illustration1.setTexture(m_cardsIllustration,gf::RectF::fromPositionSize({ (1.f / numberImageW) * i , (1.f/numberImageH) * j }, { 1.f / numberImageW, 1.f/numberImageH }));
        illustration1.setPosition({position.x+spaceBetweenCard,position.y});
        illustration1.setAnchor(gf::Anchor::Center);


        gf::RoundedRectangleShape card(sizeCard);
        card.setColor(gf::Color::White);
        card.setRadius(22);
        card.setOutlineColor(gf::Color::Black);
        card.setPosition(position);


        gf::Text cardName("Chameau", m_cardsFont, instructionsCharacterSize);
        cardName.setColor(gf::Color::Black);
        cardName.setPosition({position.x, position.y});
        cardName.setAnchor(gf::Anchor::Center);

        gf::Text cardDescription("Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement.", m_cardsFont, instructionsCharacterSize/1.5);
        cardDescription.setColor(gf::Color::Black);
        cardDescription.setPosition({position.x, position.y});
        cardDescription.setParagraphWidth(200-20);
        cardDescription.setAlignment(gf::Alignment::Center);

        target.draw(card,states);
        target.draw(illustration1,states);
        target.draw(cardName,states);
        target.draw(cardDescription,states);
    }
}

int CardsEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    return 0;
}