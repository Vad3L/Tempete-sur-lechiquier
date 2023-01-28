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
	
	m_principalView = gf::ExtendView({ 0, 0 }, { 1600, 900 });
	m_principalView.setViewport(gf::RectF::fromPositionSize({ 0., 0.f}, { 1.f, 1.f }));
	m_principalView.setCenter(game.getRenderer().getSize()/2);

	m_views.addView(m_principalView);
	m_principalView.onFramebufferSizeChange(game.getRenderer().getSize());
	m_views.setInitialFramebufferSize({game.getRenderer().getSize()});
}

void ZoomCardScene::doProcessEvent([[maybe_unused]] gf::Event& event) {
	m_views.processEvent(event);	
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
	target.setView(getHudView());
	gf::RoundedRectangleShape background(target.getSize());
	background.setColor(gf::Color::fromRgba32(0,0,0,200));

	target.draw(background, states);

	target.setView(m_principalView);

	if(m_cardZoom.m_num != -1) {
		gf::Coordinates coords(target);

		int bubbleX = (10.f/16.f)*1600;

		unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.04f);

		gf::Text bubbleTittle("Description de la Carte "+m_cardEntity.m_card.m_name, m_rulesFont, 35);	
		bubbleTittle.setColor(gf::Color::Yellow);
		bubbleTittle.setPosition(coords.getRelativePoint({ 0.66f, 0.25f}));
		bubbleTittle.setAnchor(gf::Anchor::TopCenter);
		bubbleTittle.setOutlineColor(gf::Color::Black);
		bubbleTittle.setOutlineThickness(titleCharacterSize/6.f);


		gf::RoundedRectangleShape infoBubble(gf::Vector2i(bubbleX,600));
		infoBubble.setColor(gf::Color::White);
		infoBubble.setRadius(22);
		infoBubble.setOutlineColor(gf::Color::Black);
		infoBubble.setPosition(coords.getRelativePoint({0.40f,0.21f}));
		//infoBubble.setAnchor(gf::Anchor::Center);
		
		gf::Text cardDescription(m_cardEntity.m_card.m_description, m_rulesFont, 25);
	
		cardDescription.setColor(gf::Color::Black);
		cardDescription.setPosition(coords.getRelativePoint({0.66f,0.5f}));
		cardDescription.setParagraphWidth(bubbleX-40);
		cardDescription.setAlignment(gf::Alignment::Center);
		cardDescription.setAnchor(gf::Anchor::Center);


		std::string timeToPlay;
		switch(m_cardEntity.m_card.m_turn){
			case Turn::AVANT_COUP:
				timeToPlay = "Jouez cette carte avant votre coup";
				break;
			case Turn::APRES_COUP:
				timeToPlay = "Jouez cette carte après votre coup"; 
				break;
			case Turn::BOTH:
				timeToPlay = "Jouez cette carte avant ou après votre coup";
				break;
			case Turn::DURING_TOUR_ADVERSE:
				timeToPlay = "Jouez cette carte après le coup adverse";
				break;
			default :
				timeToPlay = "Cette carte n'est pas implem";
		}

		gf::Text cardTimeToPlay(timeToPlay, m_rulesFont, 25);
	
		cardTimeToPlay.setColor(gf::Color::Black);
		cardTimeToPlay.setPosition(coords.getRelativePoint({0.66f,0.725f}));
		cardTimeToPlay.setParagraphWidth(bubbleX-40);
		cardTimeToPlay.setAlignment(gf::Alignment::Center);
		cardTimeToPlay.setAnchor(gf::Anchor::Center);

		m_cardEntity.draw(target, states, m_cardZoom, gf::Vector2f(0.15f, 0.75f), 2); // 2 = zoom
		target.draw(infoBubble, states);
		target.draw(bubbleTittle,states);
		target.draw(cardDescription,states);
		target.draw(cardTimeToPlay,states);

	}
}

void ZoomCardScene::onActivityChange(bool active) {
	if(active) {
		gf::Log::debug("scene zoom active\n");
		m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());
	}
}