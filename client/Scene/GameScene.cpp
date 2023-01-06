#include "GameScene.hpp"

#include <gf/Color.h>
#include <gf/Log.h>
#include <iostream>
#include <gf/Sleep.h>

#include "../GameHub.hpp"

GameScene::GameScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_quitAction("quit")
, m_fullscreenAction("Fullscreen")
, m_network()
, m_packet()
, m_boardEntity(game.resources, m_gameData)
, m_tableBoardEntity(game.resources, m_gameData)
{
    setClearColor(gf::Color::Black);
    
    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

  	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  	addAction(m_fullscreenAction);

    addWorldEntity(m_boardEntity);
    addWorldEntity(m_tableBoardEntity);

    m_boardView = gf::LockedView(game.getRenderer().getSize()/2, {8*m_gameData.m_sizeSquare+5.f, 8*m_gameData.m_sizeSquare+5.f});
    addView(m_boardView);
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

}

void GameScene::doProcessEvent(gf::Event& event) {
    bool click = false;
    
    switch (event.type){
    //    case gf::EventType::MouseMoved:
    //    m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
    //    break;
        case gf::EventType::MouseButtonPressed:
            click = true;
            break;
        case gf::EventType::Resized :
            m_boardView.setCenter(event.resize.size/2);
            break;
    }
    
    if(m_gameData.m_myTurn && click) { // m_gameData.m_myTurn
        gf::Vector2i v = m_boardEntity.getTransformCaseSelected(m_game.getWindow().getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, m_boardView));
        
        bool coupPionEnded = m_gameData.m_plateau.setMovement(m_gameData.m_myColor, v);
        
        if(coupPionEnded) {

            CoupReq coup;
            coup.posStart.x = m_gameData.m_plateau.coordCaseSelected.x;
            coup.posStart.y = m_gameData.m_plateau.coordCaseSelected.y;
            coup.posEnd.x = v.x;
            coup.posEnd.y = v.y;
            
            m_network.send(coup);
            
            m_network.queue.wait(m_packet);
            
            assert(m_packet.getType() == CoupRep::type);
            
            auto coupRep = m_packet.as<CoupRep>();
            
            // move piece
            if(coupRep.err == CodeRep::NONE) { // coup valide
                
                std::cout << "------COUP MOI------" << std::endl;
                m_gameData.m_plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece.isMoved = true;
                m_gameData.m_plateau.movePieces(m_gameData.m_plateau.coordCaseSelected, v);
                ChessColor colAdv = (m_gameData.m_myColor == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
                m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(colAdv);
                m_gameData.m_plateau.prettyPrint();

                m_gameData.m_plateau.coordCaseSelected = gf::Vector2i(-1,-1);
                m_gameData.m_plateau.moveAvailable.clear();	
                
                m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
                m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
                
                m_gameData.m_myTurn = !m_gameData.m_myTurn;
                m_gameData.m_plateau.prisePassant = false;
            }else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
                std::cout << "------COUP MOI INVALIDE------" << std::endl;
            }
        }
    }else {
        
        
    }
}

void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    
    target.setView(getHudView());
    m_tableBoardEntity.render(target, states);

    target.setView(m_boardView);
    m_boardEntity.render(target, states);
    
    target.setView(getHudView());
}

void GameScene::doUpdate(gf::Time time) {
    if(!m_gameData.m_myTurn && m_network.queue.poll(m_packet)) {
            
        //m_network.queue.wait(m_packet);
        assert(m_packet.getType() == CoupRep::type);
        
        auto coupAdv = m_packet.as<CoupRep>();
        
        if(coupAdv.err == CodeRep::NONE) { // coup valide
            
            std::cout << "------COUP ADVERSE------" << std::endl;
            m_gameData.m_plateau.state[coupAdv.posStart.y * 8 + coupAdv.posStart.x].piece.isMoved = true;
            m_gameData.m_plateau.movePieces(gf::Vector2i(coupAdv.posStart.x, coupAdv.posStart.y), gf::Vector2i(coupAdv.posEnd.x, coupAdv.posEnd.y));
            m_gameData.m_plateau.playerInEchec = m_gameData.m_plateau.isInEchec(m_gameData.m_myColor);
            m_gameData.m_plateau.prettyPrint();

            m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posStart.x,coupAdv.posStart.y));
            m_gameData.m_plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posEnd.x,coupAdv.posEnd.y));
                
            m_gameData.m_myTurn = !m_gameData.m_myTurn;	
            m_gameData.m_plateau.prisePassant = false;
        }else if(coupAdv.err == CodeRep::COUP_NO_VALIDE) {
            std::cout << "------COUP ADVERSE INVALIDE------" << std::endl;
        }
    }
}

void GameScene::onActivityChange(bool active){
    if(active){
        m_boardView.setCenter(m_game.getRenderer().getSize()/2);
		m_network.connect("localhost","43771");
        gf::sleep(gf::milliseconds(500));
        assert(m_network.isConnected());

        m_network.queue.wait(m_packet);
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