#include "BoardEntity.hpp"

#include<iostream>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

BoardEntity::BoardEntity(gf::ResourceManager& resources, Plateau &plateau)
: m_font(resources.getFont("DejaVuSans.ttf"))
, m_backgroundTexture(resources.getTexture("ChessSheet.png"))
, m_sizeSquare(56.f)
, m_caseSelected(-1)
,m_myColor(ChessColor::WHITE)
,m_plateau(plateau)
{
    m_backgroundTexture.setSmooth(true);
}

void BoardEntity::update([[maybe_unused]] gf::Time time) {
}

void BoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);
        
    gf::Vector2f beginPlateau(coords.getWindowSize()/2-4*m_sizeSquare);
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);
    

    bool myTurn = false;
    if(m_plateau.lastCoup.size() != 0) {
        myTurn = (m_plateau.state[m_plateau.lastCoup.back().y*8+m_plateau.lastCoup.back().x].piece.getColor() != m_myColor);
    }
    
    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    int incr = (m_myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], m_font, 20);
        
        text.setPosition(gf::Vector2f(beginPlateau.col-m_sizeSquare/2 + (i*m_sizeSquare), (beginPlateau.height)+(8.5*m_sizeSquare)));
        text.setAnchor(gf::Anchor::Center);
        text.setColor(gf::Color::fromRgba32(238,198,108));
        target.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(gf::Vector2f(beginPlateau.col - m_sizeSquare/2, (beginPlateau.height - m_sizeSquare/2)+(i*m_sizeSquare)));
        
        target.draw(text, states);
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
		
        sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));

        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(gf::Vector2f(beginPlateau.col-(tabW[(int)i]%4)*15 + (-1.25f * m_sizeSquare) , beginPlateau.height + (tabW[(int)i]/4)*15 + (i * m_sizeSquare)));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(gf::Vector2f(beginPlateau.col + (tabB[(int)i]%4)*15 + (8.25f * m_sizeSquare) , beginPlateau.height + (tabB[(int)i]/4)*15 + (i * m_sizeSquare)));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 8.f));
        target.draw(sprite, states);
    }
    
    //draw plateau 
    for  (Case &c : m_plateau.state) {
        int x = c.position.x;
        int y = c.position.y;
        gf::Vector2i pos(x, y);

        gf::RectangleShape shape({ m_sizeSquare-2.5f, m_sizeSquare -2.5f});
        shape.setPosition(gf::Vector2f(beginPlateau.col+m_sizeSquare/2 + ((float)x * m_sizeSquare), beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
        
        // if case selected
        if(pos == m_plateau.coordCaseSelected) {
            
            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
        }else // if my king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() == m_myColor && myTurn && m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else // if adv king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() != m_myColor && !myTurn && m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else if(m_plateau.lastCoup.size() >= 2 && (pos == m_plateau.lastCoup.back() || pos == m_plateau.lastCoup[m_plateau.lastCoup.size()-2])) {

            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
        }else {
            
            if (y % 2 == 0) {
                if (x % 2 == 0) {
                    shape.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f}));
                }
                else {
                    shape.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
            } else {
                if (x % 2 == 0) {
                    shape.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
                else {
                    shape.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f }));
                }
            }
        }

        shape.setAnchor(gf::Anchor::Center);
        shape.setOutlineColor(gf::Color::fromRgba32(85,60,40));
        shape.setOutlineThickness(2.5f);
        
        target.draw(shape);
        
        // draw pieces
        if (c.piece.getType() != ChessPiece::NONE) {
            gf::Sprite sprite;
            float i = (float)c.piece.getType();
            float j = (int)(c.piece.getColor())/4.f;
            
            if(c.piece.getType() == ChessPiece::PAWN && (y == 0 || y == 7)) {
                
                int mul = (c.piece.getColor() == ChessColor::WHITE) ? 1: -1;
                int tab[8] = {-1*mul,-1*mul,-1*mul, 1*mul, 1*mul, 1*mul, 1*mul, -1*mul};
                for(int z = 0; z < 4 ; z++) {
                    sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * (z+1), j }, { (1.f / numberPiece), 0.25f }));
                    sprite.setPosition(gf::Vector2f(beginPlateau.col + m_sizeSquare/2 + ((float)x * m_sizeSquare)+(m_sizeSquare/4)*tab[z*2] , beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)+(m_sizeSquare/4)*tab[z*2+1]));
                    sprite.setScale((1.f / (6.5f*2.f)));
                    sprite.setAnchor(gf::Anchor::Center);
                    
                    if (m_myColor == ChessColor::BLACK) {
                    //sprite.setRotation(gf::Pi);
                    }

                    target.draw(sprite); 
                }
            }else {

                sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(gf::Vector2f(beginPlateau.col + m_sizeSquare/2 + ((float)x * m_sizeSquare) , beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
                sprite.setScale((1.f / 6.5f));
                sprite.setAnchor(gf::Anchor::Center);

                if (m_myColor == ChessColor::BLACK) {
                    //sprite.setRotation(gf::Pi);
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
            c.setPosition(gf::Vector2f(beginPlateau.col+m_sizeSquare/2 + ((float)x * m_sizeSquare), beginPlateau.height+m_sizeSquare/2 + ((float)y * m_sizeSquare)));
            target.draw(c);
        } 
   }
 
}

gf::Vector2i BoardEntity::getTransformCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    
    gf::Vector2i v(-1,-1); 
    gf::Vector2f beginPlateau(sizeWindows/2-4*m_sizeSquare);
    
    if(true) {
        v.y = (int)((mouseCoord.height-beginPlateau.height)/(m_sizeSquare));
        v.x = (int)((mouseCoord.col-beginPlateau.col)/(m_sizeSquare));
    }
    if(v.y < 0 || v.x < 0) {
        v.x = v.y = -1;
    }

    return v;
}


void BoardEntity::setColor(ChessColor color) {
    m_myColor = color;
}
