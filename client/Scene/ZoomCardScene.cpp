#include "ZoomCardScene.hpp"
#include "../GameHub.hpp"

ZoomCardScene::ZoomCardScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("Quit")
, m_fullscreenAction("FullScreen")
, m_cardEntity(game.resources)
, m_cardsFont(game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_rulesFont(game.resources.getFont("fonts/RifficFree-Bold.ttf"))
{
	setClearColor(gf::Color::Black);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	m_quitAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_quitAction);
	
}

void ZoomCardScene::doProcessEvent([[maybe_unused]] gf::Event& event) {
	
}

void ZoomCardScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_quitAction.isActive()) {
		m_game.popScene(); 
	}
}

void ZoomCardScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	if(m_cardZoom.m_num != -1) {
		gf::Coordinates coords(target);

		gf::RoundedRectangleShape background(target.getSize());
		background.setColor(gf::Color::fromRgba32(0,0,0,200));

		int bubbleX = 1000;


		unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.04f);

		gf::Text bubbleTittle("Description de la Carte "+m_cardEntity.m_card.m_name, m_rulesFont, 30);	
		bubbleTittle.setColor(gf::Color::Yellow);
		bubbleTittle.setPosition(coords.getRelativePoint({ 0.675f, 0.25f}));
		bubbleTittle.setAnchor(gf::Anchor::TopCenter);
		bubbleTittle.setOutlineColor(gf::Color::Black);
		bubbleTittle.setOutlineThickness(titleCharacterSize/5.f);


		gf::RoundedRectangleShape infoBubble(gf::Vector2i(bubbleX,600));
		infoBubble.setColor(gf::Color::White);
		infoBubble.setRadius(22);
		infoBubble.setOutlineColor(gf::Color::Black);
		infoBubble.setPosition(coords.getRelativePoint({0.40f,0.21f}));
		//infoBubble.setAnchor(gf::Anchor::Center);
		
		gf::Text cardDescription(m_cardEntity.m_card.m_description, m_rulesFont, 20);
	
		cardDescription.setColor(gf::Color::Black);
		cardDescription.setPosition(coords.getRelativePoint({0.41f,0.5f}));
		cardDescription.setParagraphWidth(bubbleX-40);
		cardDescription.setAlignment(gf::Alignment::Center);


		std::string timeToPlay;
		switch(m_cardEntity.m_card.m_turn){
			case Turn::AVANT_COUP:
				timeToPlay = "Jouez cette carte avant votre coup";
				break;
			case Turn::APRES_COUP:
				timeToPlay = "Jouez cette carte après votre coup"; 
				break;
			case Turn::BOTH:
				timeToPlay = "Jouez cette carte quand vous voulez";
				break;
			case Turn::DURING_TOUR_ADVERSE:
				timeToPlay = "Jouez cette carte après le coup adverse";
				break;
			default :
				timeToPlay = "Cette carte n'est pas implem";
		}

		gf::Text cardTimeToPlay(timeToPlay, m_rulesFont, 20);
	
		cardTimeToPlay.setColor(gf::Color::Black);
		cardTimeToPlay.setPosition(coords.getRelativePoint({0.575f,0.725f}));
		cardTimeToPlay.setParagraphWidth(bubbleX-40);
		cardTimeToPlay.setAlignment(gf::Alignment::Justify);

		target.draw(background, states);
		m_cardEntity.draw(target, states, m_cardZoom, gf::Vector2f(0.15f, 0.75f), 2); // 2 = zoom
		target.draw(infoBubble, states);
		target.draw(bubbleTittle,states);
		target.draw(cardDescription,states);
		target.draw(cardTimeToPlay,states);

	}
}
