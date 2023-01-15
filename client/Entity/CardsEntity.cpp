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
    constexpr float instructionsCharacterSize = 0.075f;
    constexpr float spaceBetweenCard = 0.22f;

    m_gameData.m_main[0] = Card("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::NONE,29);
    m_gameData.m_main[1] = Card("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::NONE,29);
    m_gameData.m_main[2] = Card("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::NONE,29);
    m_gameData.m_main[3] = Card("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::NONE,29);
    m_gameData.m_main[4] = Card("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::NONE,29);
    
    int cpt = 0;
    for (Card &c : m_gameData.m_main) {
        if(c.m_num == -1) {
            continue;
        }
        
        int i = c.m_num%numberImageW;
        int j = c.m_num/numberImageW;
       
        gf::Vector2f position = coordsCard.getRelativePoint({ -0.53f+ spaceBetweenCard*cpt, 0.3f });

        gf::RectangleShape illustration1({sizeCard.x/1.5f, sizeCard.x/1.5f});
        illustration1.setTexture(m_cardsIllustration,gf::RectF::fromPositionSize({ (1.f / numberImageW) * i , (1.f/numberImageH) * j }, { 1.f / numberImageW, 1.f/numberImageH }));
        illustration1.setPosition({position.x+sizeCard.x/2.f,position.y+sizeCard.y/5.f});
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

        cpt++;
    }
}

int CardsEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    return 0;
}