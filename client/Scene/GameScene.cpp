#include "GameScene.hpp"

#include <gf/Log.h>
#include <iostream>

#include "../GameHub.hpp"

GameScene::GameScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("quit")
, m_fullscreenAction("Fullscreen")
, m_texture1Action("Texture1")
, m_texture2Action("Texture2")
, m_boardEntity(game.resources, m_gameData)
, m_tableBoardEntity(game.resources, m_gameData)
, m_promotion(false)
{
    setClearColor(gf::Color::Black);
    
    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

  	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  	addAction(m_fullscreenAction);
	
    m_texture1Action.addScancodeKeyControl(gf::Scancode::Num1);
  	addAction(m_texture1Action);

    m_texture2Action.addScancodeKeyControl(gf::Scancode::Num2);
  	addAction(m_texture2Action);

	m_boardView = gf::ExtendView({ 0, 0 }, { 203, 203 });
	m_boardView.setViewport(gf::RectF::fromPositionSize({ 0.3f, 0.3f}, { 0.4f, 0.4f }));
    m_tableBoardView = gf::ExtendView({ 0, 0 }, { 200, 200 });
	m_tableBoardView.setViewport(gf::RectF::fromPositionSize({ 0.2f, 0.2f}, { 0.6f, 0.6f }));
    
	m_views.addView(m_boardView);
    m_views.addView(m_tableBoardView);
    
	m_views.setInitialFramebufferSize({game.getRenderer().getSize()});
}

void GameScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
        return;
    }

    if (m_quitAction.isActive()) {
        m_game.replaceScene(m_game.play);
    }

    if (m_fullscreenAction.isActive()) {
        window.toggleFullscreen();
    }

    if(m_texture1Action.isActive()) {
        m_tableBoardEntity.m_numTexture = 0;
        m_boardEntity.m_numTexture = 0;
    }

    if(m_texture2Action.isActive()) {
        m_tableBoardEntity.m_numTexture = 1;
        m_boardEntity.m_numTexture = 1;
    }
}

void GameScene::doProcessEvent(gf::Event& event) {
    bool click = false;
	m_views.processEvent(event);
    
	if (!m_gameStart) { return; }

    switch (event.type) {
        case gf::EventType::MouseButtonPressed:
        click = true;
        break;
    }
    
    if(m_gameData.m_myTurn && click) { 
        gf::Vector2i v = m_boardEntity.getTransformCaseSelected(m_boardView.getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_boardView));
        if(v.x == -1 || v.y == -1) {
		    return;
	    }
        
        if(m_promotion) {

            PromotionRep promo;

            promo.pos.x = v.x;
            promo.pos.y = (m_gameData.m_myColor == ChessColor::WHITE ? 0: 7);

            ChessPiece choice = m_boardEntity.getChoice(m_boardView.getSize(),v);
            promo.choice = choice;
            std::cout << "envoie au serveur la promotion du pion en " << v.y << "," << v.x << "avec comme choice "<< (int)promo.choice<< std::endl;
            m_game.m_network.send(promo);
           
        }else {
            std::cout << "je suis dans le else\n";
            bool coupPionEnded = m_gameData.m_plateau.setMovement(m_gameData.m_myColor, v);
        
            if(coupPionEnded) {
                CoupRep coup;
                coup.posStart.x = m_gameData.m_plateau.coordCaseSelected.x;
                coup.posStart.y = m_gameData.m_plateau.coordCaseSelected.y;
                coup.posEnd.x = v.x;
                coup.posEnd.y = v.y;
                
                m_gameData.m_plateau.coordCaseSelected = gf::Vector2i(-1,-1);
                m_gameData.m_plateau.moveAvailable.clear();	
                    
                m_game.m_network.send(coup);
            }
        }
    }
}

void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    
    target.setView(m_tableBoardView);
    m_tableBoardEntity.render(target, states);

    target.setView(m_boardView);
    m_boardEntity.render(target, states);
    
    target.setView(getHudView());
}

void GameScene::doUpdate(gf::Time time) {
    
    if(!m_game.m_network.queue.poll(m_packet)) {
		return;
	}
	
	if (m_packet.getType() == PartieRep::type) {
		auto repPartie = m_packet.as<PartieRep>();
		if (repPartie.err == CodeRep::GAME_START) {
			gf::Log::debug("Game start\n");
            m_gameStart = true;
			return;
		} else if (repPartie.err == CodeRep::GAME_END) {
            gf::Log::debug("Game end\n");
            m_gameStart = false;
            if(repPartie.status == ChessStatus::WIN && repPartie.coulPion != m_gameData.m_myColor) {
                m_gameData.m_gameStatus = ChessStatus::LOOSE;    
            }else {
			        m_gameData.m_gameStatus = repPartie.status;
                }
			return;
		}
	}     

    if(m_packet.getType() == CoupRep::type) {
        gf::Log::debug("coup recu serveur\n");
        auto coupRep = m_packet.as<CoupRep>();
	
        // move piece
        if(coupRep.err == CodeRep::NONE) { // coup valide
            
            gf::Log::debug("------COUP CORRECT------\n");
            Piece pieceStart =  m_gameData.m_plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece;
            pieceStart.isMoved= true;

            m_gameData.m_plateau.movePieces(gf::Vector2i(coupRep.posStart.x, coupRep.posStart.y), gf::Vector2i(coupRep.posEnd.x, coupRep.posEnd.y));

            ChessColor c = !m_gameData.m_myColor;
            if (m_gameData.m_myTurn) {
                m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(c);
            } else {
                m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(m_gameData.m_myColor);
            }

            m_gameData.m_plateau.prettyPrint();
            
            m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
            m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
            
            if(pieceStart.getType() == ChessPiece::PAWN &&( coupRep.posEnd.y == 0 || coupRep.posEnd.y == 7)) {
                m_promotion = true;
            }else {
                m_gameData.m_myTurn = !m_gameData.m_myTurn;
                m_promotion = false;
            }
            
        }else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
            gf::Log::debug("------COUP INVALIDE------\n");
        }
    }

    if(m_packet.getType() == PromotionRep::type) {
        gf::Log::debug("promo recu serveur\n");
        assert(m_promotion);

        auto promoRep = m_packet.as<PromotionRep>();
        if(promoRep.err == CodeRep::NONE) {
            gf::Log::debug("------PROMOTION CORRECT------\n");
            
            m_gameData.m_plateau.promotionPiece(gf::Vector2i(promoRep.pos.x, promoRep.pos.y), promoRep.choice);

            ChessColor c = !m_gameData.m_myColor;
            if (m_gameData.m_myTurn) {
                m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(c);
            } else {
                m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(m_gameData.m_myColor);
            }

            m_gameData.m_plateau.prettyPrint();
            m_gameData.m_myTurn = !m_gameData.m_myTurn;
            m_promotion = false;
        }else {
            gf::Log::debug("------PROMOTION INVALIDE------\n");
        }
    }
}

void GameScene::onActivityChange(bool active) {
    if(active){
        m_gameStart = false;
        m_gameData.m_gameStatus =  ChessStatus::ON_GOING;
        m_views.setInitialFramebufferSize(m_game.getRenderer().getSize());

        m_game.m_network.queue.wait(m_packet);
        assert(m_packet.getType() == PartieRep::type);

        auto repPartie = m_packet.as<PartieRep>();
        assert(repPartie.err == CodeRep::NONE);

        m_gameData.m_myColor = repPartie.coulPion;

        if (m_gameData.m_myColor == ChessColor::WHITE) {
            m_gameData.m_myTurn = true;
        }
        else {
            m_gameData.m_myTurn = false;
        }

        std::cout << "Vous jouez la couleur : " << (int)m_gameData.m_myColor << std::endl;

        if(m_gameData.m_myColor == ChessColor::BLACK) {
            m_boardView.setRotation(gf::Pi);
        }
    }
}
