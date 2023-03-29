#include "BoardEntity.h"

#include <gf/Coordinates.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>

namespace tsl {

    BoardEntity::BoardEntity(gf::ResourceManager& resources, GameModel &model)
    : m_model(model)
    , m_backgroundTexture1(resources.getTexture("images/ChessSheet.png"))
    , m_backgroundTexture2(resources.getTexture("images/ChessSheet2.png"))
    , m_backgroundTexture3(resources.getTexture("images/ChessSheet3.png"))
    , m_target(resources.getTexture("images/Target.png"))
    {
        m_backgroundTexture1.setSmooth(true);
        m_backgroundTexture2.setSmooth(true);
        m_backgroundTexture3.setSmooth(true);
        m_target.setSmooth(true);
    }

    void BoardEntity::update([[maybe_unused]] gf::Time time) {
    }


    void BoardEntity::render(gf::RenderTarget &target, const gf::RenderStates & m_states) {
        
        int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);
        gf::Coordinates coords(gf::Vector2i(400.f , 400.f));


        float sizeSquare = coords.getRelativeSize(gf::vec(0.0f, 1.f/8.f)).height;
        float sizeLine = 2.5f;

        bool myTurn = false;
        ChessColor lastColor = ChessColor::WHITE;
        
        if(m_model. board.m_lastCoup.size() >= 2) {
            lastColor = m_model. board. m_state[m_model. board.m_lastCoup.back().y *8 +m_model. board.m_lastCoup.back().x].m_piece.getColor();
        }

        myTurn = (lastColor != m_model.chessColor);

        gf::Texture &texture = (m_model.theme == 1) ? m_backgroundTexture1 : (m_model.theme == 2) ? m_backgroundTexture2 : m_backgroundTexture3;
        
        gf::Vector2f pieceToPromuteCoords(-1);
        ChessColor pieceToPromuteColor = ChessColor::NONE;
        
        //draw plateau 
        for  (Case &c : m_model. board. m_state) {
            int x = c.m_position.x;
            int y = c.m_position.y;
            gf::Vector2i pos(x, y);
            
            gf::RectangleShape shape({ sizeSquare-sizeLine, sizeSquare-sizeLine});
            shape.setPosition(coords.getRelativePoint({-0.5f + (float)x * 1.f/8.f + 1/16.f, -0.5f + (float)y * 1.f/8.f + 1/16.f}));
            
            // if case selected
            if(pos == m_model. board.m_coordCaseSelected) {
                shape.setColor(gf::Color::fromRgba32(250, 190, 88, 200));
            } else if (c.m_piece.getType() == ChessPiece::KING && c.m_piece.getColor() == m_model.chessColor && myTurn && m_model. board.m_playerInEchec) {
                shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
            } else if(c.m_piece.getType() == ChessPiece::KING && c.m_piece.getColor() != m_model.chessColor && !myTurn && m_model. board.m_playerInEchec) {
                shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
            } else if(m_model. board.m_lastCoup.size() >= 2 && (pos == m_model. board.m_lastCoup.back() || pos == m_model. board.m_lastCoup[m_model. board.m_lastCoup.size()-2])) {
                shape.setColor(gf::Color::fromRgba32(250, 190, 88, 200));
            }else {

                if (y % 2 == 0) {
                    if (x % 2 == 0) {
                        shape.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f}));
                    }
                    else {
                        shape.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                    }
                } else {
                    if (x % 2 == 0) {
                        shape.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                    }
                    else {
                        shape.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f }));
                    }
                }
            }

            shape.setAnchor(gf::Anchor::Center);
            if(m_model.theme == 0) {
                shape.setOutlineColor(gf::Color::fromRgba32(85,60,40));
            }else {
                shape.setOutlineColor(gf::Color::fromRgba32(60,60,60));
            }
            
            shape.setOutlineThickness(sizeLine);

            target.draw(shape,  m_states);

            gf::Sprite sprite;
            // draw pieces
            if (c.m_piece.getType() != ChessPiece::NONE) {
    
                
                float i = (float)c.m_piece.getType();
                float j = ((int)c.m_piece.getColor())/4.f;
                

                if(c.m_piece.getType() == ChessPiece::PAWN && !myTurn && ((y == 0 && c.m_piece.getColor() == ChessColor::WHITE) || (y == 7 && c.m_piece.getColor() == ChessColor::BLACK ))) {
                    
                    pieceToPromuteCoords.x = x;
                    pieceToPromuteCoords.y = y;
                    pieceToPromuteColor = c.m_piece.getColor();

                }else {
                    
                    sprite.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                    sprite.setPosition(coords.getRelativePoint({ -0.5f + 1.f/16.f + x * 1.f/8.f, -0.5f + 1.f/16.f + y * 1.f/8.f }));
                    sprite.setScale(1.f/7.f);
                    sprite.setAnchor(gf::Anchor::Center);
                    
                    if (m_model.chessColor == ChessColor::BLACK) {
                        sprite.setRotation(gf::Pi);
                    }

                    target.draw(sprite,  m_states);
                }	 
            }
            
            if(std::find(m_model. board.m_casesClicked.begin(),m_model. board.m_casesClicked.end(), pos) != m_model. board.m_casesClicked.end()) {
                shape.unsetTexture();
                switch (m_model.gamePhase.getCurrentPhase()) {
                    case Phase::AVANT_COUP:
                        shape.setColor(gf::Color::Yellow);
                        break;
                    case Phase::APRES_COUP:
                        shape.setColor(gf::Color::Green);
                        break;
                    default :
                        shape.setColor(gf::Color::Black);
                }
                
                target.draw(shape,  m_states);
                target.draw(sprite,  m_states);
                
                shape.setColor(gf::Color::White);
                shape.setTexture(m_target, gf::RectF::fromPositionSize({0.f, 0.f}, { 1.f, 1.f}));
                target.draw(shape,  m_states);
            }

            // draw move authorized
            auto it = std::find(m_model. board.m_moveAvailable.begin(), m_model. board.m_moveAvailable.end(), gf::Vector2i(x, y));
            if(it != m_model. board.m_moveAvailable.end()) {
            
                gf::CircleShape circle(9.f);
                circle.setColor(gf::Color::Gray(.6f));
                circle.setPosition(coords.getRelativePoint({ -0.5f + 1.f/16.f + x * 1.f/8.f, -0.5f + 1.f/16.f + y * 1.f/8.f }));
                circle.setAnchor(gf::Anchor::Center);
                target.draw(circle,  m_states);
            }
        }

        if(m_model. board.m_promotion){
            gf::RectangleShape rectangle({sizeSquare-sizeLine,((sizeSquare*4)-sizeLine)});
            rectangle.setColor(gf::Color::White);
            rectangle.setOutlineColor(gf::Color::Black);
            rectangle.setOutlineThickness(sizeLine);
            
            /*std::cout << "piece y :" << (float)pieceToPromuteCoords.y * 1.f/8.f + 1/16.f << std::endl;
            std::cout << "sizeSquare :" << sizeSquare << std::endl;*/

            //int mul = (pieceToPromuteColor == ChessColor::WHITE) ? 1 : -1;

            rectangle.setPosition(coords.getRelativePoint({-0.5f + (float)pieceToPromuteCoords.x * 1.f/8.f + 1/16.f, -0.5f + (float)pieceToPromuteCoords.y * 1.f/8.f + (pieceToPromuteColor == ChessColor::WHITE ? (1/4.f) : (-1/8.f))}));
            rectangle.setAnchor(gf::Anchor::Center);
            target.draw(rectangle,  m_states);
            
            for(int z = 0; z < 4 ; z++) {
                //std::cout << << std::endl;
                gf::Sprite sprite;
                sprite.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * (z+1), (int)(pieceToPromuteColor)/4.f }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(coords.getRelativePoint({ (float)pieceToPromuteCoords.x * 1.f/8.f + -0.5f + 1.f/16.f, (float)(pieceToPromuteCoords.y+z)* 1.f/8.f + -0.5f + (pieceToPromuteColor == ChessColor::WHITE ? (1/16.f) : (-1/3.2f))}));
                sprite.setScale(1.f/7.f);
                sprite.setAnchor(gf::Anchor::Center);
                
                if (m_model.chessColor == ChessColor::BLACK) {
                    sprite.setRotation(gf::Pi);
                }
                target.draw(sprite,  m_states);
            }
        }
    }

    gf::Vector2i BoardEntity::getCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
        
        gf::Vector2i v(-1,-1); 
        float min = std::min(sizeWindows.height, sizeWindows.width);
        
        if(mouseCoord.y + min/2.f<0 || mouseCoord.x + min/2.f<0 || mouseCoord.y + min/2.f>min || mouseCoord.x + min/2.f>min ) {
            gf::Log::debug("caseSelectionne: [ligne/col] %i , %i \n",v.y,v.x);
            return v;
        }

        v.y = (int)((8.f/min) * (mouseCoord.y + min/2.f)); //+min/2 pour ramener les coordonées dans le positif 
        v.x = (int)((8.f/min) * (mouseCoord.x + min/2.f));
        
        gf::Log::debug("caseSelectionne: [ligne/col] %i , %i \n",v.y,v.x);
        return v;
    }

    ChessPiece BoardEntity::getChoice([[maybe_unused]] gf::Vector2i sizeWindows, gf::Vector2i clickCoord) {
        ChessPiece p = ChessPiece::NONE;
        
        if(m_model.chessColor == ChessColor::BLACK){
            clickCoord.y -= 4;
        } 
    
        switch(clickCoord.y){
            case 0:
                p = ChessPiece::QUEEN;
                break;
            case 1:
                p = ChessPiece::BISHOP;
                break;
            case 2:
                p = ChessPiece::KNIGHT;
                break;
            case 3:
                p = ChessPiece::ROOK;
                break;
        }

        return p;
    }

}