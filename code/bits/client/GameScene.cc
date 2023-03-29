#include "GameScene.h"
#include "GameHub.h"

#include "../common/Protocol.h"

namespace tsl {

    GameScene::GameScene(GameHub& game, Network &network)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_network(network)
    , m_model(m_game.m_model)
    , m_choice(ButtonChoice::None)
    , m_buttons(m_game.resources,  m_choice, m_game.m_model)
    , m_info(m_game.resources, m_game.m_model)
    , m_poseEntity(m_game.resources, m_game.m_model)
    , m_boardEntity(m_game.resources, m_game.m_model)
    , m_tableBoardEntity(m_game.resources, m_game.m_model)
    , m_poseActionRight("PoseActionRight")
    , m_poseActionLeft("PoseActionLeft")
    , m_discardActionRight("DiscardActionRight")
    , m_quitAction("Quit")
    , m_poseRight(false)
    , m_poseLeft(false)
    , m_discardRight(false)
    {

        setClearColor(gf::Color::Black);

        m_poseActionRight.addKeycodeKeyControl(gf::Keycode::A);
        addAction(m_poseActionRight);
        
        m_poseActionLeft.addKeycodeKeyControl(gf::Keycode::Z);
        addAction(m_poseActionLeft);

        m_discardActionRight.addKeycodeKeyControl(gf::Keycode::E);
        addAction(m_discardActionRight);
        
        m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	    addAction(m_quitAction);

        addHudEntity(m_buttons);
        addHudEntity(m_info);
        addHudEntity(m_poseEntity);
        addHudEntity(m_boardEntity);
        addHudEntity(m_tableBoardEntity);

        m_boardView = gf::ExtendView({ 0, 0 }, { 403, 403 });
        m_boardView.setViewport(gf::RectF::fromPositionSize({ 0.275f, 0.125f}, { 0.45f, 0.45f }));

        m_tableBoardView = gf::ExtendView({ 0, 0 }, { 500, 500 });
        m_tableBoardView.setViewport(gf::RectF::fromPositionSize({ 0.15f, 0.0f}, { 0.7f, 0.7f }));

        m_cardsView = gf::ExtendView({ 0, 0 }, { 1200, 300 });
        m_cardsView.setViewport(gf::RectF::fromPositionSize({ 0.125f, 0.7f}, { 0.75f, 0.3f }));

        m_views.addView(m_boardView);
        m_views.addView(m_tableBoardView);
        m_views.addView(m_cardsView);
        
        m_views.setInitialFramebufferSize({m_game.getRenderer().getSize()});

        clickEndTurn.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
    	clickEndTurn.setVolume(FxsVolume);
        
        clickplayCard.setBuffer(gAudioManager().getSound("sounds/PlayCard.ogg"));
	    clickplayCard.setVolume(FxsVolume);

        takePiece.setBuffer(gAudioManager().getSound("sounds/TakePiece.ogg"));
	    takePiece.setVolume(FxsVolume);

        movePiece.setBuffer(gAudioManager().getSound("sounds/MovePiece.ogg"));
        movePiece.setVolume(FxsVolume);

        checkPiece.setBuffer(gAudioManager().getSound("sounds/CheckPiece.ogg"));
        checkPiece.setVolume(FxsVolume);

        drawCard.setBuffer(gAudioManager().getSound("sounds/DrawCard.ogg"));
        drawCard.setVolume(FxsVolume);
    }

    void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
        if (!isActive()) {
            return;
        }

        if (m_quitAction.isActive()) {
            m_game.pushScene(*m_game.quit);
            pause();
	    }

        if(m_poseEntity.m_cardPlaying != -1 && m_poseActionRight.isActive()){
            gf::Log::debug("pushed poseRight\n");
            m_poseRight = !m_poseRight;
        }
        
        if(m_poseEntity.m_cardPlaying != -1 && m_poseActionLeft.isActive()){
            gf::Log::debug("pushed poseLeft\n");
            m_poseLeft = !m_poseLeft;
        }
        
        if(m_poseEntity.m_cardDiscard != -1 && m_discardActionRight.isActive()){
            gf::Log::debug("pushed discardRight\n");
            m_discardRight = !m_discardRight;
        }
    }

    void GameScene::doProcessEvent(gf::Event& event) {
        m_views.processEvent(event);

        if(!isActive() || m_model.chessStatus != ChessStatus::ON_GOING) {
            return;
        }

        bool clickLeft = false;
	    bool clickRight = false;
	    bool mouseMoved = false;
        
        int c = -1;

        if(m_poseEntity.m_cardPlaying != -1 && m_poseLeft) {
            m_poseLeft = !m_poseLeft;
            c = m_poseEntity.m_cardPlaying;
        }

        if(m_poseEntity.m_cardDiscard != -1  && m_discardRight) {
            m_discardRight = !m_discardRight;
            c = m_poseEntity.m_cardDiscard;
        }
        
        if(c != -1) {
            m_game.zoom->m_numCard = c;
            m_game.pushScene(*m_game.zoom);
            pause();
        }


        if(m_poseEntity.m_cardPlaying != -1 && m_poseRight) {
            m_poseEntity.returnCardHand();
            m_model.gamePhase.setCurrentSubPhase(SubPhase::NONE);
            m_model.board.m_casesClicked.clear();
            m_poseRight = !m_poseRight;
            return;
        }
        
        switch (event.type) {
            case gf::EventType::MouseMoved:
                m_buttons.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                mouseMoved = true;
                
                break;
            case gf::EventType::MouseButtonPressed:
                m_buttons.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                m_buttons.triggerAction();                
                
                if(event.mouseButton.button == gf::MouseButton::Left) {
                    clickLeft = true;
                }else {
                    clickRight = true;
                }
                break;
            default :
                break;
        }

        if(mouseMoved) {            
            m_model.hoverCard = m_info.getSelectedCard(m_cardsView.getSize(), m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, m_cardsView));
        }

        if(clickRight) {
            int numCarte = m_info.getSelectedCard(m_cardsView.getSize(), m_game.computeWindowToGameCoordinates(event.mouseButton.coords, m_cardsView));
            
            if(numCarte != -1) {
                m_game.zoom->m_numCard = m_model.cards[numCarte];
                m_game.pushScene(*m_game.zoom);
                pause();	
            }
        }

        if(!clickLeft) {
            return;
        }


        Phase currentPhase = m_model.gamePhase.getCurrentPhase();

        if(m_model.gamePhase.getCurrentSubPhase() == SubPhase::CLIQUER_CASES) {
            gf::Vector2i v = m_boardEntity.getCaseSelected(m_boardView.getSize(), m_game.computeWindowToGameCoordinates(event.mouseButton.coords, m_boardView));
            if(v.x == -1 || v.y == -1) {
                return;
            }
            
            auto it = std::find(m_model.board.m_casesClicked.begin(), m_model.board.m_casesClicked.end(), v);
            if(it != m_model.board.m_casesClicked.end()) {
                m_model.board.m_casesClicked.erase(it);
                return;
            }
            
            if(m_model.board.m_casesClicked.size() < m_model.deck[m_poseEntity.m_cardPlaying].m_nbClickPossible) {
                m_model.board.m_casesClicked.push_back(v);
            }
            return;
        }

        if((currentPhase == Phase::APRES_COUP || currentPhase == Phase::AVANT_COUP) && !m_model.board.m_promotion) {
            int numCarte = m_info.getSelectedCard(m_cardsView.getSize(), m_game.computeWindowToGameCoordinates(event.mouseButton.coords, m_cardsView));

            if(numCarte!=-1) {
                
                bool playable = m_model.deck[m_model.cards[numCarte]].m_isPlayable(m_model.board, currentPhase);

                if(playable && m_poseEntity.m_cardPlaying == -1) {
                    m_poseEntity.m_cardPlaying = m_model.cards[numCarte];
                    if(m_model.deck[m_model.cards[numCarte]].m_action == Action::CHOOSE_CASES) {
                        m_model.gamePhase.setCurrentSubPhase(SubPhase::CLIQUER_CASES);
                    }
                    m_model.cards[numCarte] = -1;
                    m_model.board.m_coordCaseSelected = gf::Vector2i(-1);
                    m_model.board.m_moveAvailable.clear();
                }
            }
        }
        
        if(currentPhase==Phase::COUP || (currentPhase==Phase::AVANT_COUP  && m_poseEntity.m_cardPlaying==-1)) { 
            gf::Vector2i v = m_boardEntity.getCaseSelected(m_boardView.getSize(), m_game.computeWindowToGameCoordinates(event.mouseButton.coords, m_boardView));
            if(v.x == -1 || v.y == -1) {
                //gf::Log::info("clique en dehors plateau\n");
                return;
            }

            if(m_model.board.m_promotion) {
                
                PromotionRep promo;

                promo.pos.x = v.x;
                promo.pos.y = (m_model.chessColor == ChessColor::WHITE ? 0: 7);

                ChessPiece choice = m_boardEntity.getChoice(m_boardView.getSize(),v);
                promo.choice = choice;
                gf::Log::debug("envoie au serveur la promotion du pion en %i,%i avec comme choice %i\n",v.y,v.x,(int)promo.choice);
                m_network.send(promo);
            
            }else {
                
                bool coupPionEnded = m_model.board.setMovement(v);
                if(m_model.board.m_coordCaseSelected != gf::Vector2i(-1) && !coupPionEnded) {
                    takePiece.play();
                }

                if(coupPionEnded) {
                    CoupRep coup;
                    coup.posStart.x = m_model.board.m_coordCaseSelected.x;
                    coup.posStart.y = m_model.board.m_coordCaseSelected.y;
                    coup.posEnd.x = v.x;
                    coup.posEnd.y = v.y;
                    
                    m_model.board.m_coordCaseSelected = gf::Vector2i(-1,-1);
                    m_model.board.m_moveAvailable.clear();	
                        
                    m_network.send(coup);
                }
            }
        }
    }

    void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        target.setView(getHudView());
        
        target.setView(m_tableBoardView);
        m_tableBoardEntity.render(target, states);

        target.setView(m_boardView);
        m_boardEntity.render(target, states);

        target.setView(m_cardsView);
        m_info.render(target, states);

        target.setView(getHudView());
        m_poseEntity.render(target, states);

        m_buttons.render(target, states);    
    }

    void GameScene::doUpdate([[maybe_unused]] gf::Time time) {
        
        switch (m_choice) {
            case ButtonChoice::EndTurn:
                gf::Log::debug("EndTurn pressed!\n");
                if(m_model.gamePhase.getCurrentPhase()==Phase::APRES_COUP) {
                    clickEndTurn.play();
                    m_model.gamePhase.setCurrentPhase(Phase::PAS_MON_TOUR);
                    CardRep cardRep;
                    cardRep.err = CodeRep::NO_CARD;
                    m_network.send(cardRep);
                    m_model.board.m_turnTo = !m_model.board.m_turnTo;
                    m_poseEntity.returnCardHand();
                    m_model.gamePhase.setCurrentSubPhase(SubPhase::NONE);
                    m_model.board.m_casesClicked.clear();
                }
                break;
            case ButtonChoice::PlayCard:
                gf::Log::debug("PlayCard pressed!\n");
                if(m_poseEntity.m_cardPlaying != -1) {

                    CardRep cardRep;
                    cardRep.err = CodeRep::CARD;
                    cardRep.card = -1; 
                    
                    cardRep.poses = m_model.board.m_casesClicked;

                    for(std::size_t i=0; i < m_model.cards.size(); i++) {
                        if(m_model.cards[i] == -1) {
                            cardRep.card = i; 
                            break;
                        }
                    }
                    
                    clickplayCard.play();
                    gf::Log::debug("envoie au serveur la card %li\n", cardRep.card);
                    m_network.send(cardRep);
                }
                break;

                case ButtonChoice::None:
                    break;
        }
        
        m_choice = ButtonChoice::None;

        gf::Packet packet;

        while (m_network.queue.poll(packet)) {
            switch (packet.getType()) {
                
                case EndGame::type: {
                    gf::Log::info("game end recu serveur\n");
                    auto repPartie = packet.as<EndGame>();
                                        
                    if(repPartie.chessStatus == ChessStatus::WIN && repPartie.chessColor != m_model.chessColor) {
                        m_model.chessStatus = ChessStatus::LOOSE;	
                    }else {
                        m_model.chessStatus = repPartie.chessStatus;
                    }

                    break;
                }

                
                case StartTurn::type: {
                    gf::Log::info("Mon tour commence\n");
                    auto turn = packet.as<StartTurn>();
                    m_model.gamePhase.setCurrentPhase(Phase::AVANT_COUP);
                    assert(m_model.gamePhase.getNbCardPlay()==0);
                    m_model.board.m_turnTo = turn.colorPlayer;

                    break;
                }

                case CoupRep::type: {
                    gf::Log::info("coup recu serveur\n");
                    auto coupRep = packet.as<CoupRep>();
                
                    // move m_piece
                    if(coupRep.err == CodeRep::NONE) { // coup valide
                        
                        gf::Log::debug("------COUP CORRECT------\n");
                        Piece pieceStart =  m_model.board.m_state[coupRep.posStart.y * 8 + coupRep.posStart.x].m_piece;
                        pieceStart.m_isMoved= true;

                        m_model.board.movePieces(gf::Vector2i(coupRep.posStart.x, coupRep.posStart.y), gf::Vector2i(coupRep.posEnd.x, coupRep.posEnd.y));

                        bool playerInEchecB = m_model.board.m_playerInEchec;
                        m_model.board.m_playerInEchec = m_model.board.isInEchec(!m_model.board.m_turnTo);

                        if(!playerInEchecB && m_model.board.m_playerInEchec) {
                            checkPiece.play();
                        }else {
                            movePiece.play();
                        }

                        m_model.board.prettyPrint();
                        
                        m_model.board.m_lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
                        m_model.board.m_lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
                        
                        m_model.board.m_allPositions.push_back(m_model.board.getFen());	
                        m_poseEntity.returnCardHand();

                        if(pieceStart.getType() == ChessPiece::PAWN &&( coupRep.posEnd.y == 0 || coupRep.posEnd.y == 7)) {
                            m_model.board.m_promotion = true;
                        }else {
                            if(m_model.gamePhase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
                                
                                if(m_model.gamePhase.getNbCardPlay() >= m_model.gamePhase.getLimNbCard()) {
                                    m_model.gamePhase.setCurrentPhase(Phase::PAS_MON_TOUR);
                                    m_model.board.m_turnTo = !m_model.board.m_turnTo;	
                                }else {
                                    m_model.gamePhase.setCurrentPhase(Phase::APRES_COUP);
                                }
                            }
                            m_model.board.m_promotion = false;
                        }
                        
                        
                    }else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
                        gf::Log::debug("------COUP INVALIDE------\n");
                    }

                    break;
                }

                case SendCard::type: {    
                    gf::Log::info("carte recu serveur\n");
            
                    auto deckRep = packet.as<SendCard>();
                    	
                    for(int i=0; i< 5; i++) {	
                        if(m_model.cards[i] == -1) {
                            m_model.cards[i] = deckRep.num;
                        }
                    }
                

                    break;
                }
                case PromotionRep::type: {
                    gf::Log::info("promo recu serveur\n");
                    //assert(m_model.board.m_promotion);

                    auto promoRep = packet.as<PromotionRep>();
                    if(promoRep.err == CodeRep::NONE) {
                        gf::Log::debug("------PROMOTION CORRECT------\n");
                        
                        m_model.board.promotionPiece(gf::Vector2i(promoRep.pos.x, promoRep.pos.y), promoRep.choice);

                        bool playerInEchecB = m_model.board.m_playerInEchec;
                        m_model.board.m_playerInEchec = m_model.board.isInEchec(!m_model.board.m_turnTo);

                        if(!playerInEchecB && m_model.board.m_playerInEchec) {
                            checkPiece.play();
                        }
                        
                        m_model.board.m_allPositions.push_back(m_model.board.getFen());
                        m_poseEntity.returnCardHand();

                        m_model.board.prettyPrint();
                        if(m_model.gamePhase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
                            assert(m_model.board.m_promotion);
                            if(m_model.gamePhase.getNbCardPlay() >= m_model.gamePhase.getLimNbCard()) {

                                m_model.gamePhase.setCurrentPhase(Phase::PAS_MON_TOUR);
                                m_model.board.m_turnTo = !m_model.board.m_turnTo;	
                            }else {
                                m_model.gamePhase.setCurrentPhase(Phase::APRES_COUP);
                            }
                        }
                        m_model.board.m_promotion = false;
                    }else {
                        gf::Log::debug("------PROMOTION INVALIDE------\n");
                    }

                    break;
                }

                case Debug::type: {
                    gf::Log::info("\n\nrecu plateau serveur mode debug \n\n");
                    auto p = packet.as<Debug>();
                    gf::Log::info("\n\nplateau moi: %s \n\nplateau serveur: %s \n\n",m_model.board.getFen().c_str(), p.board.c_str());
                    assert(p.board==m_model.board.getFen());

                    break;
                }

                case CardRep::type: {
                    auto carteRep = packet.as<CardRep>();
                
                    if(carteRep.err == CodeRep::NONE) {
                        gf::Log::debug("------CARTE VALIDE------ %i\n", carteRep.num);

                        m_model.deck[carteRep.num].m_execute(m_model.board, carteRep.poses);
                    
                        m_model.board.m_playerInEchec = m_model.board.isInEchec(!m_model.board.m_turnTo);

                        m_model.board.m_allPositions.push_back(m_model.board.getFen());

                        m_poseEntity.m_cardDiscard = carteRep.num;

                        m_model.board.m_casesClicked.clear();

                        if(m_model.gamePhase.getCurrentPhase() != Phase::PAS_MON_TOUR) {
                            m_model.gamePhase.nextPhaseCard(m_model.deck[carteRep.num]);
                            
                            m_model.gamePhase.setCurrentSubPhase(SubPhase::NONE);

                            if(m_model.board.hasPromotion()) {
                                m_model.gamePhase.setCurrentPhase(Phase::COUP);
                                m_model.board.m_promotion = true;
                            }

                            m_poseEntity.m_cardPlaying = -1;
                            if (m_model.gamePhase.getCurrentPhase() == Phase::PAS_MON_TOUR) { //carte mais fin a mon tour
                                m_model.board.m_turnTo = !m_model.board.m_turnTo;	
                            }

                            clickplayCard.stop();
                            drawCard.play();
                        }
                        
                    }else {
                        gf::Log::debug("------CARTE INVALIDE------\n");
                    }	
                
                    break;
                }
            }
        }
    }

    void GameScene::onActivityChange(bool active) {
        gf::Log::debug("en pause = %d et active = %d\n",isPaused(), isActive());
        if(active && m_model.gamePhase.getCurrentPhase() == Phase::NONE){
            gf::Log::debug("scene game active\n");

            clickEndTurn.setVolume(FxsVolume);
            clickplayCard.setVolume(FxsVolume);
            
            takePiece.setVolume(FxsVolume);
            movePiece.setVolume(FxsVolume);
            checkPiece.setVolume(FxsVolume);
            drawCard.setVolume(FxsVolume);            

            m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());

            if(m_model.chessColor == ChessColor::BLACK) {
                m_boardView.setRotation(gf::Pi);
            }

            if (m_model.chessColor == ChessColor::WHITE) {
				m_model.gamePhase.setCurrentPhase(Phase::AVANT_COUP);
			}
			else {
				m_model.gamePhase.setCurrentPhase(Phase::PAS_MON_TOUR);
			}

        }else if(!active && isPaused()){
            gf::Log::debug("scene game desactive\n");
            m_network.disconnect();

            m_model.reset();

            m_boardView.setRotation(0.f);
            m_poseEntity.m_cardDiscard = -1;
            m_poseEntity.m_cardPlaying = -1;
   
        }
    }

    void GameScene::doShow() {
        m_buttons.m_endTurn.setDefault();
        m_buttons.m_playCard.setDefault();
    }
}