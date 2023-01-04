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
, m_myTurn(true)
, m_myColor(ChessColor::WHITE)
, m_plateau()
, m_network()
, m_packet()
, m_boardEntity(game.resources, m_plateau)
{
    //setClearColor(gf::Color::Blue);

    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

  	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  	addAction(m_fullscreenAction);

    addWorldEntity(m_boardEntity);
    
    
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
    
    switch (event.type)
    {
    //    case gf::EventType::MouseMoved:
    //    m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
    //    break;
        case gf::EventType::MouseButtonPressed:
        click = true;
        break;
    }
    
    if(m_myTurn && click) { // m_myTurn
        gf::Vector2i v = m_boardEntity.getTransformCaseSelected(m_game.getWindow().getSize(), m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords, getHudView()));
        
        bool coupPionEnded = m_plateau.setMovement(m_myColor, v);
        
        if(coupPionEnded) {

            CoupReq coup;
            coup.posStart.x = m_plateau.coordCaseSelected.x;
            coup.posStart.y = m_plateau.coordCaseSelected.y;
            coup.posEnd.x = v.x;
            coup.posEnd.y = v.y;
            
            m_network.send(coup);
            
            m_network.queue.wait(m_packet);
            
            assert(m_packet.getType() == CoupRep::type);
            
            auto coupRep = m_packet.as<CoupRep>();
            
            // move piece
            if(coupRep.err == CodeRep::NONE) { // coup valide
                
                std::cout << "------COUP MOI------" << std::endl;
                m_plateau.state[coupRep.posStart.y * 8 + coupRep.posStart.x].piece.isMoved = true;
                m_plateau.movePieces(m_plateau.coordCaseSelected, v);
                ChessColor colAdv = (m_myColor == ChessColor::WHITE) ? ChessColor::BLACK : ChessColor::WHITE;
                m_plateau.playerInEchec = m_plateau.isInEchec(colAdv);
                m_plateau.prettyPrint();

                m_plateau.coordCaseSelected = gf::Vector2i(-1,-1);
                m_plateau.moveAvailable.clear();	
                
                m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posStart.x,coupRep.posStart.y));
                m_plateau.lastCoup.push_back(gf::Vector2i(coupRep.posEnd.x,coupRep.posEnd.y));
                
                m_myTurn = !m_myTurn;
                m_plateau.prisePassant = false;
            }else if(coupRep.err == CodeRep::COUP_NO_VALIDE) {
                std::cout << "------COUP MOI INVALIDE------" << std::endl;
            }
        }
    }else {
        
        
    }
}

void GameScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
    
    auto screenSize_2 = m_game.getRenderer().getSize()/2;
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);

    target.setView(getHudView());
    gf::RectangleShape tableCloth(gf::Vector2f(672.f, 560.f));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setPosition(screenSize_2);
    tableCloth.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
    target.draw(tableCloth);
    
    
    m_boardEntity.render(target, states);
    
    std::string turn = (m_myTurn) ? std::string("It\'s your turn !") : std::string("It's opponent's turn !");
    gf::Text text(turn, m_game.resources.getFont("DroidSans.ttf"), 20);
    
    text.setPosition(gf::Vector2f(screenSize_2.x, screenSize_2.y-250.f));
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::Black);
    target.draw(text, states);
    
    target.setView(getHudView());
}

void GameScene::doUpdate(gf::Time time) {
    if(!m_myTurn && m_network.queue.poll(m_packet)) {
            
        //m_network.queue.wait(m_packet);
        assert(m_packet.getType() == CoupRep::type);
        
        auto coupAdv = m_packet.as<CoupRep>();
        
        if(coupAdv.err == CodeRep::NONE) { // coup valide
            
            std::cout << "------COUP ADVERSE------" << std::endl;
            m_plateau.state[coupAdv.posStart.y * 8 + coupAdv.posStart.x].piece.isMoved = true;
            m_plateau.movePieces(gf::Vector2i(coupAdv.posStart.x, coupAdv.posStart.y), gf::Vector2i(coupAdv.posEnd.x, coupAdv.posEnd.y));
            m_plateau.playerInEchec = m_plateau.isInEchec(m_myColor);
            m_plateau.prettyPrint();

            m_plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posStart.x,coupAdv.posStart.y));
            m_plateau.lastCoup.push_back(gf::Vector2i(coupAdv.posEnd.x,coupAdv.posEnd.y));
                
            m_myTurn = !m_myTurn;	
            m_plateau.prisePassant = false;
        }else if(coupAdv.err == CodeRep::COUP_NO_VALIDE) {
            std::cout << "------COUP ADVERSE INVALIDE------" << std::endl;
        }
    }
}

void GameScene::onActivityChange(bool active){
    if(active){
        
        m_network.connect("localhost","4555");
        gf::sleep(gf::milliseconds(500));
        assert(m_network.isConnected());

        m_network.queue.wait(m_packet);
        assert(m_packet.getType() == PartieRep::type);

        auto repPartie = m_packet.as<PartieRep>();

        assert(repPartie.err == CodeRep::NONE);

        m_myColor = repPartie.coulPion;

        if (m_myColor == ChessColor::WHITE) {
            m_myTurn = true;
        }
        else {
            m_myTurn = false;
        }

        std::cout << "Vous jouez la couleur : " << (int)m_myColor << std::endl;
        m_boardEntity.setColor(m_myColor);
        if(m_myColor == ChessColor::BLACK) {
            //l_view.setRotation(gf::Pi);
        }
    }
}