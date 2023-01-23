#include "PoseEntity.hpp"

PoseEntity::PoseEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
, m_accessories(resources.getTexture("images/AccesoriesCards.png"))
, m_gameData(gameData)
, m_poseNameFont(resources.getFont("fonts/DejaVuSans.ttf"))
, m_cardEntity(resources, gameData)
{
	m_cardsIllustration.setSmooth(true);
	m_accessories.setSmooth(true);
	m_cardPose = Card();
	m_cardDiscard = Card();
}

void PoseEntity::update([[maybe_unused]] gf::Time time) {
}

void PoseEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

	gf::Coordinates coords(target);
	
	gf::Vector2i sizeCard = gf::Vector2i(200+20,300+20);

	gf::Vector2f position1 = coords.getRelativePoint({ 0.2f, 0.4f });
	gf::Vector2f position2 = coords.getRelativePoint({ 0.8f, 0.4f });
	
	gf::RoundedRectangleShape contour(sizeCard);
	contour.setColor(gf::Color::Black);
	contour.setOutlineThickness(6.f);
	contour.setRadius(22);
	contour.setPosition(position1);
	contour.setAnchor(gf::Anchor::Center);

	gf::Text poseName("Carte à jouée", m_poseNameFont, 16.f);
	poseName.setPosition({position1.x,position1.y-sizeCard.y/2.f-2.5f});
	poseName.setAnchor(gf::Anchor::Center);

	switch (m_gameData.m_phase.getCurrentPhase()) {
		case Phase::AVANT_COUP :
			contour.setOutlineColor(gf::Color::Yellow);
			poseName.setColor(gf::Color::Yellow);
			break;
		case Phase::APRES_COUP :
			contour.setOutlineColor(gf::Color::Green);
			poseName.setColor(gf::Color::Green);
			break;
		case Phase::PAS_MON_TOUR:
			contour.setOutlineColor(gf::Color::Cyan);
			poseName.setColor(gf::Color::Cyan);
			break;
		default :
			contour.setOutlineColor(gf::Color::White);
			poseName.setColor(gf::Color::White);
			break;
	}
	if(m_gameData.m_gameStatus != ChessStatus::ON_GOING) {
		contour.setOutlineColor(gf::Color::White);
		poseName.setColor(gf::Color::White);
	}

	gf::RectangleShape placementTexte;
	placementTexte.setSize({130.f,14.0f});
	placementTexte.setColor(gf::Color::Black);
	placementTexte.setPosition({position1.x,position1.y-sizeCard.y/2.f-2.5f});
	placementTexte.setAnchor(gf::Anchor::Center);

	target.draw(contour, states);
	target.draw(placementTexte,states);
	target.draw(poseName,states);

	if(m_cardPose.m_num!=-1) {
		m_cardEntity.render(target, states, m_cardPose, 0.237f, 0.94f);
	}

	//défausse

	contour.setOutlineColor(gf::Color::White);
	contour.setPosition(position2);
	contour.setAnchor(gf::Anchor::Center);

	placementTexte.setSize({100.f,14.0f});
	placementTexte.setPosition({position2.x,position2.y-sizeCard.y/2.f-2.5f});
	placementTexte.setAnchor(gf::Anchor::Center);

	poseName.setString("Défausse");
	poseName.setColor(gf::Color::White);
	poseName.setPosition({position2.x,position2.y-sizeCard.y/2.f-5.f});
	poseName.setAnchor(gf::Anchor::Center);

	target.draw(contour, states);
	target.draw(placementTexte,states);
	target.draw(poseName,states);
	
	if(m_cardDiscard.m_num!=-1) {
		m_cardEntity.render(target, states, m_cardDiscard, 1.197f, 0.94f);
	}
}

void PoseEntity::returnCardHand() {
	if(m_cardPose.m_num == -1) {
		return;
	}

	for(auto &c : m_gameData.m_main ) {
		if(c.m_num == -1) {
			std::swap(m_cardPose, c);
			break;
		}
	}
}

bool PoseEntity::clickIsInCardPose(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
	gf::Log::info("coord case clicker carte pose %i et %i\n", mouseCoord.x,mouseCoord.y);
	int x = mouseCoord.x;
	int y = mouseCoord.y;
	// 270 < x < 500
	// 270 < y < 600
	if(x>270 && x<500 && y>270 && y<600) {
		gf::Log::info("on a pas clicker sur la carte posée\n");
		return true;
	}
	gf::Log::info("on a pas clicker sur la carte posée\n");
	return false;
}