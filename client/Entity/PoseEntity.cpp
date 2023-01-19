#include "PoseEntity.hpp"

PoseEntity::PoseEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
, m_accessories(resources.getTexture("images/AccesoriesCards.png"))
, m_gameData(gameData)
, m_poseNameFont(resources.getFont("fonts/DejaVuSans.ttf"))
{
	m_cardsIllustration.setSmooth(true);
	m_accessories.setSmooth(true);
	m_cardPose = Card();
}

void PoseEntity::update([[maybe_unused]] gf::Time time) {
}

void PoseEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

	gf::Coordinates coords(target);
	
	gf::Vector2i sizeCard = gf::Vector2i(200+10,300+10);

	gf::Vector2f position = coords.getRelativePoint({ 0.2f, 0.4f });
	
	gf::RoundedRectangleShape contour(sizeCard);
	contour.setColor(gf::Color::fromRgba32(0.f, 0.f, 0.f, 0.f));
	
	gf::Text poseName("Carte jouée", m_poseNameFont, 16.f);
	poseName.setPosition({position.x,position.y-sizeCard.y/2.f-2.5f});
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
	
	contour.setOutlineThickness(6.f);
	contour.setRadius(22);
	contour.setPosition(position);
	contour.setAnchor(gf::Anchor::Center);

	gf::RectangleShape placementTexte;
	placementTexte.setSize({130.f,14.0f});
	placementTexte.setColor(gf::Color::Black);
	placementTexte.setPosition({position.x,position.y-sizeCard.y/2.f-2.5f});
	placementTexte.setAnchor(gf::Anchor::Center);

	// TODO DESSINER LA CARTE si il ya en a une
	if(m_cardPose.m_num!=-1) {
		gf::Log::debug("je dessine une carte\n");
	}
	
	target.draw(contour, states);
	target.draw(placementTexte,states);
	target.draw(poseName,states);
}
