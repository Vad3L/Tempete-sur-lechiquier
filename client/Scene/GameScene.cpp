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
{
    auto screenSize_2 = m_game.getRenderer().getSize()/2;
    
    m_sizeSquare = 56.f;
    m_beginPlateau = gf::Vector2f(screenSize_2-4*m_sizeSquare);
    m_plateauSize = gf::Vector2f(m_sizeSquare * 8+5.f, m_sizeSquare * 8+5.f);

    //setClearColor(gf::Color::Blue);

    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

  	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
  	addAction(m_fullscreenAction);

    m_boardView = gf::LockedView(screenSize_2, m_plateauSize);
    addView(m_boardView);

    m_gameView = gf::LockedView(screenSize_2, gf::Vector2f(m_sizeSquare*12, m_sizeSquare*10));
    addView(m_gameView);
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
        gf::Vector2i v = transformInSelectedCase(event.mouseButton.coords);
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

    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);

    target.setView(m_gameView);
    auto screenSize = m_game.getRenderer().getSize();
    
    gf::RectangleShape tableCloth(gf::Vector2f(m_plateauSize.x + 4 * m_sizeSquare, m_plateauSize.y + 2 * m_sizeSquare));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setPosition(screenSize/2);
    tableCloth.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
    target.draw(tableCloth);
    
    std::string turn = (m_myTurn) ? std::string("It\'s your turn !") : std::string("It's opponent's turn !");
    gf::Text text(turn, m_game.resources.getFont("DroidSans.ttf"), 20);
    
    text.setPosition(gf::Vector2f(screenSize.x/2.f, (m_beginPlateau.height)-25.f));
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::Black);
    target.draw(text);

    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    int incr = (m_myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], m_game.resources.getFont("DroidSans.ttf"), 20);
        
        text.setPosition(gf::Vector2f(m_beginPlateau.col-m_sizeSquare/2 + (i*m_sizeSquare), (m_beginPlateau.height)+(8.5*m_sizeSquare)));
        text.setAnchor(gf::Anchor::Center);
        text.setColor(gf::Color::fromRgba32(238,198,108));
        target.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(gf::Vector2f(m_beginPlateau.col - m_sizeSquare/2, (m_beginPlateau.height - m_sizeSquare/2)+(i*m_sizeSquare)));
        
        target.draw(text);
        (m_myColor == ChessColor::WHITE) ? incr-- : incr++; 
    }
    
    // draw bin
    int tabW[numberPiece];
    int tabB[numberPiece];
    for(int i = 0 ; i < numberPiece ; ++i){
        tabB[i] = 0;
        tabW[i] = 0;
    }
    
    for(Piece &pi : m_plateau.bin) {
        gf::Sprite sprite;
        float i = (float)pi.getType();
		float j = (int)(pi.getColor())/4.f;
		
        sprite.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));

        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(gf::Vector2f(m_beginPlateau.col-(tabW[(int)i]%4)*15 + (-1.25f * m_sizeSquare) , m_beginPlateau.height + (tabW[(int)i]/4)*15 + (i * m_sizeSquare)));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(gf::Vector2f(m_beginPlateau.col + (tabB[(int)i]%4)*15 + (8.25f * m_sizeSquare) , m_beginPlateau.height + (tabB[(int)i]/4)*15 + (i * m_sizeSquare)));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 8.f));
        target.draw(sprite);
    }
    
    target.setView(m_boardView);
    
    //draw plateau 
    for  (Case &c : m_plateau.state) {
        int x = c.position.x;
        int y = c.position.y;
        gf::Vector2i pos(x, y);

        gf::RectangleShape shape({ m_sizeSquare-2.5f, m_sizeSquare -2.5f});
        shape.setPosition(gf::Vector2f(m_beginPlateau.col+m_sizeSquare/2 + ((float)x * m_sizeSquare), m_beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
        
        // if case selected
        if(pos == m_plateau.coordCaseSelected) {
            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
            //shape.setColor(gf::Color::fromRgba32(30, 144, 255, 180));
        }else // if my king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() == m_myColor && m_myTurn && m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else // if adv king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() != m_myColor && !m_myTurn && m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else if(m_plateau.lastCoup.size() >= 2 && (pos == m_plateau.lastCoup.back() || pos == m_plateau.lastCoup[m_plateau.lastCoup.size()-2])) {

            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
        }else {
            
            if (y % 2 == 0) {
                if (x % 2 == 0) {
                    shape.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f}));
                }
                else {
                    shape.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
            } else {
                if (x % 2 == 0) {
                    shape.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
                else {
                    shape.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f }));
                }
            }
        }

        
        shape.setAnchor(gf::Anchor::Center);
        shape.setOutlineColor(gf::Color::fromRgba32(85,60,40));
        shape.setOutlineThickness(2.5f);
        
        target.draw(shape);
        
        // draw piece
        if (c.piece.getType() != ChessPiece::NONE) {
            gf::Sprite sprite;
            float i = (float)c.piece.getType();
            float j = (int)(c.piece.getColor())/4.f;
            
            if(c.piece.getType() == ChessPiece::PAWN && (y == 0 || y == 7)) {
                
                int mul = (c.piece.getColor() == ChessColor::WHITE) ? 1: -1;
                int tab[8] = {-1*mul,-1*mul,-1*mul, 1*mul, 1*mul, 1*mul, 1*mul, -1*mul};
                for(int z = 0; z < 4 ; z++) {
                    sprite.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * (z+1), j }, { (1.f / numberPiece), 0.25f }));
                    sprite.setPosition(gf::Vector2f(m_beginPlateau.col + m_sizeSquare/2 + ((float)x * m_sizeSquare)+(m_sizeSquare/4)*tab[z*2] , m_beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)+(m_sizeSquare/4)*tab[z*2+1]));
                    sprite.setScale((1.f / (6.5f*2.f)));
                    sprite.setAnchor(gf::Anchor::Center);
                    
                    if (m_myColor == ChessColor::BLACK) {
                        sprite.setRotation(gf::Pi);
                    }

                    target.draw(sprite); 
                }
            }else {

                sprite.setTexture(m_game.resources.getTexture("ChessSheet.png"), gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(gf::Vector2f(m_beginPlateau.col + m_sizeSquare/2 + ((float)x * m_sizeSquare) , m_beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
                sprite.setScale((1.f / 6.5f));
                sprite.setAnchor(gf::Anchor::Center);
                
                if (m_myColor == ChessColor::BLACK) {
                    sprite.setRotation(gf::Pi);
                }

                target.draw(sprite);
            }            
        }
   
        // draw move authorized
        auto it = std::find(m_plateau.moveAvailable.begin(), m_plateau.moveAvailable.end(), gf::Vector2i(x, y));
        if(it != m_plateau.moveAvailable.end()) {
        
            gf::CircleShape c(9.f);
            c.setColor(gf::Color::Gray(.6f));
            c.setAnchor(gf::Anchor::Center);
            c.setPosition(gf::Vector2f(m_beginPlateau.col+m_sizeSquare/2 + ((float)x * m_sizeSquare), m_beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
            target.draw(c);
        } 
   } 
   
   target.setView(getHudView());
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
        if (m_myColor == ChessColor::BLACK) {
            m_boardView.setRotation(gf::Pi);
        } 
    }
}

gf::Vector2i GameScene::transformInSelectedCase(gf::Vector2i mouseCoord) {
    gf::Vector2i mouseCoords = m_game.getRenderer().mapPixelToCoords(mouseCoord, getHudView());
    //std::cout << "la : " << ((mouseCoords.height-m_beginPlateau.height))<< ";" << (mouseCoords.col-m_beginPlateau.col) << std::endl;
    gf::Vector2i v(-1,-1); 

    if(mouseCoords.height > m_beginPlateau.height && mouseCoords.height < m_beginPlateau.height+m_plateauSize.height && mouseCoords.col > m_beginPlateau.col && mouseCoords.col < m_beginPlateau.col+m_plateauSize.col) {
        v.y = (int)((mouseCoords.height-m_beginPlateau.height)/(m_sizeSquare));
        v.x = (int)((mouseCoords.col-m_beginPlateau.col)/(m_sizeSquare));
    }

    std::cout << "caseSelectionne: [ligne/col] " << v.y << "," << v.x << std::endl;
    return v;
}

void GameScene::doUpdate(gf::Time time) {
    if(m_network.queue.poll(m_packet)) {
            
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