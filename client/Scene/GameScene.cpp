#include "GameScene.hpp"
#include "../GameHub.hpp"

GameScene::GameScene(GameHub& game, Network &network, GameData &gameData)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_network(network)
, m_gameData(gameData)
, m_quitAction("quit")
, m_fullscreenAction("Fullscreen")
, m_endTurnAction("endTurnAction")
, m_playCardAction("playCardAction")
, m_boardEntity(game.resources, m_gameData)
, m_mainEntity(game.resources, m_gameData)
, m_tableBoardEntity(game.resources, m_gameData)
, m_poseEntity(game.resources, m_gameData)
, m_promotion(false)
, m_endTurn("Fin tour", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_playCard("Activer carte", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_triggerAction("TriggerAction")
{
	setClearColor(gf::Color::Black);
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);

	m_endTurnAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_endTurnAction);

	m_playCardAction.addScancodeKeyControl(gf::Scancode::P);
	addAction(m_playCardAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_triggerAction);
	
	m_boardView = gf::ExtendView({ 0, 0 }, { 403, 403 });
	m_boardView.setViewport(gf::RectF::fromPositionSize({ 0.275f, 0.125f}, { 0.45f, 0.45f }));

	m_tableBoardView = gf::ExtendView({ 0, 0 }, { 500, 500 });
	m_tableBoardView.setViewport(gf::RectF::fromPositionSize({ 0.15f, 0.0f}, { 0.7f, 0.7f }));

	m_cardsView = gf::ExtendView({ 0, 0 }, { 1200, 300 });
	m_cardsView.setViewport(gf::RectF::fromPositionSize({ 0.125f, 0.7f}, { 0.75, 0.3f }));

	m_views.addView(m_boardView);
	m_views.addView(m_tableBoardView);
	m_views.addView(m_cardsView);

	m_views.setInitialFramebufferSize({game.getRenderer().getSize()});

	gf::Coordinates coordsWidget({500,500});
	auto setupButton = [&] (gf::TextButtonWidget& button, gf::Vector2f position,  auto callback) {
		button.setDefaultTextColor(gf::Color::fromRgba32(212,30,27,255));
		button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
		button.setDefaultBackgroundOutlineColor(gf::Color::Gray(0.7f)); 

		button.setSelectedTextColor(gf::Color::fromRgba32(212,30,27,255));
		button.setSelectedBackgroundColor(gf::Color::Gray(0.7f));
		button.setSelectedBackgroundOutlineColor(gf::Color::Gray(0.5f));
		
		button.setDisabledTextColor(gf::Color::Black);
		button.setDisabledBackgroundColor(gf::Color::Gray(0.7f));
		button.setDisabledBackgroundOutlineColor(gf::Color::Red); 

		button.setRadius(5.f);
		button.setBackgroundOutlineThickness(4.f);
		button.setPosition( coordsWidget.getRelativePoint(position));
		button.setAlignment(gf::Alignment::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_endTurn, { 0.6f, 0.25f }, [&] () {
		if(m_gameData.m_phase.getCurrentPhase()==Phase::APRES_COUP) {
			gf::Log::debug("EndTurn pressed!\n");
			m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
			CardRep cardRep;
			cardRep.err = CodeRep::NO_CARD;
			m_network.send(cardRep);
			m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;
			m_poseEntity.returnCardHand();
		}
	});

	setupButton(m_playCard, { 0.9f, 0.25f }, [&] () {
		if(m_poseEntity.m_cardPose.m_num == -1) {
			return;
		}

		CardRep cardRep;
		cardRep.err = CodeRep::CARD;
		cardRep.card = -1; 
		
		for(int i=0; i < m_gameData.m_main.size(); i++) {
			if(m_gameData.m_main[i].m_num == -1) {
				cardRep.card = i; 
				break;
			}
		}
		
		// pour l'instant pas a et b car pas de case clique
		gf::Log::debug("envoie au serveur la card %li\n", cardRep.card);
		m_network.send(cardRep);
	});
}

void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if (m_quitAction.isActive()) {
		m_game.pushScene(*m_game.quit);
		pause();
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_triggerAction.isActive()) {
		m_widgets.triggerAction();
	}

	if (m_endTurnAction.isActive()){
		gf::Log::debug("pushed endturn\n");
		m_endTurn.triggerCallback();
	}

	if(m_playCardAction.isActive()) {
		gf::Log::debug("pushed playCard\n");
		m_playCard.triggerCallback();
	}

}

void GameScene::doProcessEvent(gf::Event& event) {
	if(!isActive()) {
		return;
	}
	
	bool clickLeft = false;
	bool clickRight = false;

	m_views.processEvent(event);
	
	if (m_gameData.m_gameStatus != ChessStatus::ON_GOING) { return; }
	
	switch (event.type) {
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;

		case gf::EventType::MouseButtonPressed:
			if(event.mouseButton.button == gf::MouseButton::Left) {
				clickLeft = true;
			}else {
				clickRight = true;
			}
			break;

		}

	if(clickRight) {
		int numCarte = m_mainEntity.getCardSelected(m_cardsView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_cardsView));
		
		if(numCarte!=-1) {
			m_game.zoomCard->m_cardZoom = m_gameData.m_main[numCarte];
			m_game.pushScene(*m_game.zoomCard);
			pause();
		}
	}

	if(!clickLeft) {
		return;
	}

	if(m_poseEntity.clickIsInCardPose(getHudView().getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, getHudView()))) {
		m_poseEntity.returnCardHand();
		return;
	}

	Phase currentPhase = m_gameData.m_phase.getCurrentPhase();
	
	if(currentPhase == Phase::APRES_COUP || currentPhase == Phase::AVANT_COUP) {
		int numCarte = m_mainEntity.getCardSelected(m_cardsView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_cardsView));

		if(numCarte!=-1) {

			bool playable = m_gameData.m_main[numCarte].m_isPlayable(m_gameData.m_plateau, currentPhase);

			gf::Log::info("carte %i est jouable %i \n", numCarte, playable);

			if(playable && m_poseEntity.m_cardPose.m_num == -1) {
				m_poseEntity.m_cardPose = m_gameData.m_main[numCarte];
				m_gameData.m_main[numCarte] = Card(); 
			}
		}
	}
	
	if(currentPhase==Phase::COUP || currentPhase==Phase::AVANT_COUP) { 
		gf::Vector2i v = m_boardEntity.getCaseSelected(m_boardView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_boardView));
		if(v.x == -1 || v.y == -1) {
			return;
		}

		if(m_promotion) {

			PromotionRep promo;

			promo.pos.x = v.x;
			promo.pos.y = (m_gameData.m_myColor == ChessColor::WHITE ? 0: 7);

			ChessPiece choice = m_boardEntity.getChoice(m_boardView.getSize(),v);
			promo.choice = choice;
			gf::Log::debug("envoie au serveur la promotion du pion en %i,%i avec comme choice %i\n",v.y,v.x,(int)promo.choice);
			m_network.send(promo);
		   
		}else {

			bool coupPionEnded = m_gameData.m_plateau.setMovement(m_gameData.m_myColor, v);
		
			if(coupPionEnded) {
				CoupRep coup;
				coup.posStart.x = m_gameData.m_plateau.coordCaseSelected.x;
				coup.posStart.y = m_gameData.m_plateau.coordCaseSelected.y;
				coup.posEnd.x = v.x;
				coup.posEnd.y = v.y;
				
				m_gameData.m_plateau.coordCaseSelected = gf::Vector2i(-1,-1);
				m_gameData.m_plateau.moveAvailable.clear();	
					
				m_network.send(coup);
			}
		}
	}
}

void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	
	target.setView(m_tableBoardView);
	m_tableBoardEntity.render(target, states);

	target.setView(m_boardView);
	m_boardEntity.render(target, states);

	target.setView(m_cardsView);
	m_mainEntity.render(target, states);

	target.setView(getHudView());
	m_poseEntity.render(target, states);

	target.setView(getHudView());
	m_widgets.render(target, states);
}

void GameScene::doUpdate(gf::Time time) {
	if(!m_network.m_queue.poll(m_packet)) {
		return;
	}
	
	Phase currentPhase = m_gameData.m_phase.getCurrentPhase();

	if (m_packet.getType() == PartieRep::type) {

		auto repPartie = m_packet.as<PartieRep>();
		if (repPartie.err == CodeRep::GAME_START) {
			gf::Log::info("Jeux commence\n");
			m_gameData.m_gameStatus = ChessStatus::ON_GOING;
			
		} else if (repPartie.err == CodeRep::GAME_END) {
			gf::Log::info("Jeux Fini\n");

			if(repPartie.status == ChessStatus::WIN && repPartie.colorPion != m_gameData.m_myColor) {
				m_gameData.m_gameStatus = ChessStatus::LOOSE;	
			}else {
				m_gameData.m_gameStatus = repPartie.status;
			}

		} else if(repPartie.err == CodeRep::COLOR) {
			auto repPartie = m_packet.as<PartieRep>();

			m_gameData.m_myColor = repPartie.colorPion;

			if (m_gameData.m_myColor == ChessColor::WHITE) {
				m_gameData.m_phase.setCurrentPhase(Phase::AVANT_COUP);
			}
			else {
				m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
			}

			gf::Log::info("Vous jouez la couleur : %i\n", (int)m_gameData.m_myColor);

			if(m_gameData.m_myColor == ChessColor::BLACK) {
				m_boardView.setRotation(gf::Pi);
			}
		}else if(repPartie.err == CodeRep::TURN_START) {
			gf::Log::info("mon tour commence\n");
			//assert(m_gameData.m_phase.getCurrentPhase()==Phase::PAS_MON_TOUR);
			m_gameData.m_phase.setCurrentPhase(Phase::AVANT_COUP);
			m_gameData.m_plateau.turnTo = m_gameData.m_myColor;
		}
	}	 

	if(m_packet.getType() == CoupRep::type) {
		gf::Log::info("coup recu serveur\n");
		auto coupRep = m_packet.as<CoupRep>();
	
		// move piece
		if(coupRep.err == CodeRep::NONE) { // coup valide
			
			gf::Log::debug("------COUP CORRECT------\n");
			Piece pieceStart =  m_gameData.m_plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece;
			pieceStart.isMoved= true;

			m_gameData.m_plateau.movePieces(gf::Vector2i(coupRep.posStart.x, coupRep.posStart.y), gf::Vector2i(coupRep.posEnd.x, coupRep.posEnd.y));

			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);

			m_gameData.m_plateau.prettyPrint();
			
			m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
			m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
			
			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());	
			m_poseEntity.returnCardHand();

			if(pieceStart.getType() == ChessPiece::PAWN &&( coupRep.posEnd.y == 0 || coupRep.posEnd.y == 7)) {
				m_promotion = true;
			}else {
				if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
					m_gameData.m_phase.setCurrentPhase(Phase::APRES_COUP);
					if(m_gameData.m_phase.getNbCartePlay() !=0) {
						
						m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
						m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
					}
				}
				m_promotion = false;
			}
			
		}else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
			gf::Log::debug("------COUP INVALIDE------\n");
		}
	}

	if(m_packet.getType() == PromotionRep::type) {
		gf::Log::info("promo recu serveur\n");
		assert(m_promotion);

		auto promoRep = m_packet.as<PromotionRep>();
		if(promoRep.err == CodeRep::NONE) {
			gf::Log::debug("------PROMOTION CORRECT------\n");
			
			m_gameData.m_plateau.promotionPiece(gf::Vector2i(promoRep.pos.x, promoRep.pos.y), promoRep.choice);

			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);
			
			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());
			m_poseEntity.returnCardHand();

			m_gameData.m_plateau.prettyPrint();
			if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
				m_gameData.m_phase.setCurrentPhase(Phase::APRES_COUP);
				if(m_gameData.m_phase.getNbCartePlay() !=0) {

					m_gameData.m_phase.setCurrentPhase(Phase::PAS_MON_TOUR);
					m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
				}
			}
			m_promotion = false;
		}else {
			gf::Log::debug("------PROMOTION INVALIDE------\n");
		}
	}

	if(m_packet.getType() == DistribRep::type) {
		gf::Log::info("deck recu serveur\n");
		
		auto deckRep = m_packet.as<DistribRep>();
		
		int j = 0;
		if(deckRep.err == CodeRep::NONE) {	
			for(int i=0; i< 5; i++) {	
				if(m_gameData.m_main[i].m_num == -1) {
					m_gameData.m_main[i] = m_gameData.m_cards[deckRep.hand[j].m_num];
					j++;
				}
			}
		}
	}

	if(m_packet.getType() == CardRep::type) {
		gf::Log::info("recu carte serveur\n");
		
		auto carteRep = m_packet.as<CardRep>();
		
		if(carteRep.err == CodeRep::NONE) {
			gf::Log::debug("------CARTE VALIDE------ %i\n", carteRep.num);

			m_gameData.m_cards[carteRep.num].m_execute(m_gameData.m_plateau, carteRep.a, carteRep.b);
			
			m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(!m_gameData.m_plateau.turnTo);

			m_gameData.m_plateau.allPositions.push_back(m_gameData.m_plateau.getFen());

			m_poseEntity.m_cardDiscard = m_gameData.m_cards[carteRep.num];

			if(m_gameData.m_phase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
				m_gameData.m_phase.nextPhaseCard(m_gameData.m_cards[carteRep.num]);
				m_poseEntity.m_cardPose = Card();
				if(m_gameData.m_phase.getCurrentPhase() == Phase::PAS_MON_TOUR) { //carte mais fin a mon tour
					m_gameData.m_plateau.turnTo = !m_gameData.m_plateau.turnTo;	
				}
			}
		}else {
			gf::Log::debug("------CARTE INVALIDE------\n");
		}	
	}

	//mode debug
	if(m_packet.getType() == Debug::type) {
		gf::Log::info("\n\nrecu plateau serveur mode debug \n\n");
		auto p = m_packet.as<Debug>();
		gf::Log::info("\n\nplateau moi: %s \n\nplateau serveur: %s \n\n",m_gameData.m_plateau.getFen().c_str(), p.plateau.c_str());
		assert(p.plateau==m_gameData.m_plateau.getFen());
	}
}

void GameScene::onActivityChange(bool active) {
	gf::Log::debug("en pause = %d et active = %d\n",isPaused(), isActive());
	if(active && !isPaused()){
		gf::Log::debug("scene game active\n");
		m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());
		
	}else if(!active && isPaused()){
		gf::Log::debug("scene game desactive\n");
		m_network.deconnect();
		m_boardView.setRotation(0);
		m_gameData.reset();
	}
}

void GameScene::doShow() {
	m_widgets.clear();

	m_endTurn.setDefault();
	m_playCard.setDefault();

	m_widgets.addWidget(m_endTurn);
	m_widgets.addWidget(m_playCard);
}