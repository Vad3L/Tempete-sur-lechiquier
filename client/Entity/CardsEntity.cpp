#include "CardsEntity.hpp"




CardsEntity::CardsEntity(gf::ResourceManager& resources,GameData &gameData)
: m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_CardsFont(resources.getFont("fonts/DejaVuSans.ttf"))
, m_ilustrationSheet(resources.getTexture("images/CardIlustrationSheet.png"))
{
    m_hand.push_back(m_deck.getFirst());
}

void CardsEntity::update([[maybe_unused]] gf::Time time) {
}



void CardsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

    gf::Coordinates coordsCard(target);
    gf::Vector2f position = coordsCard.getRelativePoint({ -0.2f, 0.15f });

    
    for(int i = 0 ; i < m_hand.size(); ++i){
        
        int column = m_hand[i].getId();
        while(column > 9){
            column-=10;
        }
        float ligne = m_hand[i].getId()/10;
        gf::Log::debug("column : %i | ligne : %f\n",column,ligne);
        gf::RectangleShape illustration1({175.f,175.f});
        illustration1.setTexture(m_ilustrationSheet,gf::RectF::fromPositionSize({(1.f/10.f)*column,(1.f/14.f)*ligne},{1.f/10.f,1.f/14.f}));
        illustration1.setPosition({position.x+100,position.y+100});
        illustration1.setAnchor(gf::Anchor::Center);



        gf::RoundedRectangleShape card1({ 200.f , 300.f});
        card1.setColor(gf::Color::White);
        card1.setRadius(10);
        card1.setOutlineColor(gf::Color::Black);
        card1.setPosition(position);


        unsigned instructionsCharacterSize = coordsCard.getRelativeCharacterSize(0.01f)*2;

        gf::Text card1Name(m_hand[i].getName(), m_CardsFont, instructionsCharacterSize);
        card1Name.setColor(gf::Color::Black);
        card1Name.setPosition({position.x+100, position.y+200});
        card1Name.setAnchor(gf::Anchor::Center);

        gf::Text card1Description("Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement.", m_CardsFont, instructionsCharacterSize/1.5);
        card1Description.setColor(gf::Color::Black);
        card1Description.setPosition({position.x+10, position.y+230});
        card1Description.setParagraphWidth(200-20);
        card1Description.setAlignment(gf::Alignment::Center);

        target.draw(card1,states);
        target.draw(illustration1,states);
        target.draw(card1Name,states);
        target.draw(card1Description,states);
    }
}

int CardsEntity::getCardSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    return 0;
}