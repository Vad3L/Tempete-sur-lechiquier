#include "CardEntity.hpp"

CardEntity::CardEntity(gf::ResourceManager& resources)
: m_cardsFont(resources.getFont("fonts/DejaVuSans.ttf"))
,m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
,m_accessories(resources.getTexture("images/AccesoriesCards.png"))
{
	m_cardsIllustration.setSmooth(true);
	m_accessories.setSmooth(true);
}

void CardEntity::update([[maybe_unused]] gf::Time time) {
}

void CardEntity::draw(gf::RenderTarget &target, const gf::RenderStates &states, Card c, gf::Vector2f boxCoord, int zoom){
	m_card = c;
	int numberImageW = 10;
	int numberImageH = 14;
	gf::Coordinates coordsCard({1200,300});
	gf::Vector2i sizeCard = gf::Vector2i(200*zoom,300*zoom);
	float titleCharacterSize = zoom*(16.f);
	float instructionsCharacterSize = zoom*(14.f);

	if(c.m_num == -1) {
		return;
	}
	
	int i = c.m_num%numberImageW;
	int j = c.m_num/numberImageW;	   
	gf::Vector2f position= coordsCard.getRelativePoint({ boxCoord.x, boxCoord.y });
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
	if(c.m_name.size()>16) {
		cardName.setPosition({position.x+25*zoom, position.y+sizeCard.y/1.62f});
	}else {
		cardName.setPosition({position.x+25*zoom, position.y+sizeCard.y/1.55f});
	}
	cardName.setAlignment(gf::Alignment::Center);
	cardName.setParagraphWidth(sizeCard.x-zoom*50);

	gf::RectangleShape rect({sizeCard.x/1.1f, sizeCard.x/4.3f});
	rect.setTexture(m_accessories,gf::RectF::fromPositionSize({ 0.f  , 0.f }, { 1.f , 1.f }));
	rect.setPosition({position.x+sizeCard.x/2.f, position.y+sizeCard.y/1.6f});
	rect.setAnchor(gf::Anchor::Center);
	
	gf::RoundedRectangleShape dalto({20.f*zoom,20.f*zoom});
	dalto.setAnchor(gf::Anchor::Center);
	dalto.setPosition({position.x+sizeCard.x-(20.f*zoom), position.y+sizeCard.y/12.f});

	
	switch (c.m_turn) {
		case Turn::AVANT_COUP:
			dalto.setColor(gf::Color::Yellow);
			dalto.setRadius(50);
			rect.setColor(gf::Color::Yellow);
			break;
		case Turn::APRES_COUP:
			dalto.setColor(gf::Color::Green);
			rect.setColor(gf::Color::Green);
			break;
		case Turn::BOTH:
			dalto.setColor(gf::Color::fromRgba32(0,165,225));
			dalto.setRotation(95);
			rect.setColor(gf::Color::fromRgba32(0,165,225));
			break;
		case Turn::DURING_TOUR_ADVERSE:
			dalto.setColor(gf::Color::fromRgba32(237,101,211));
			dalto.setRadius(25);
			rect.setColor(gf::Color::fromRgba32(237,101,211));
			break;
	}
	/*if(m_gameData.m_phase.getCurrentPhase()==Phase::COUP) {
		rect.setColor(gf::Color::White);
	}*/
	
	std::string description = c.m_description; 
	
	description = description.substr(0, 60) + "...";
	

	gf::Text cardDescription(description, m_cardsFont, instructionsCharacterSize);
	
	cardDescription.setColor(gf::Color::Black);
	cardDescription.setPosition({position.x+10, position.y+sizeCard.y/1.3f});
	cardDescription.setParagraphWidth(sizeCard.x-20);
	cardDescription.setAlignment(gf::Alignment::Center);
		
	target.draw(card, states);
	target.draw(illustration, states);
	target.draw(rect, states);
	target.draw(dalto,states);
	target.draw(cardName, states);
	target.draw(cardDescription, states);
}