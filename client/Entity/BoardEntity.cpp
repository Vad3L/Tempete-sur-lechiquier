#include "BoardEntity.hpp"

#include<iostream>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

BoardEntity::BoardEntity(gf::ResourceManager& resources, GameData &gameData)
: m_font(resources.getFont("DejaVuSans.ttf"))
, m_backgroundTexture(resources.getTexture("ChessSheet.png"))
,m_gameData(gameData)
{
    m_backgroundTexture.setSmooth(true);
}

void BoardEntity::update([[maybe_unused]] gf::Time time) {
}

void BoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);
        
    gf::Vector2f beginPlateau(coords.getWindowSize()/2-4*m_gameData.m_sizeSquare);
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);
    

    bool myTurn = false;
    if(m_gameData.m_plateau.lastCoup.size() != 0) {
        myTurn = (m_gameData.m_plateau.state[m_gameData.m_plateau.lastCoup.back().y*8+m_gameData.m_plateau.lastCoup.back().x].piece.getColor() != m_gameData.m_myColor);
    }
    
    //draw plateau 
    for  (Case &c : m_gameData.m_plateau.state) {
        int x = c.position.x;
        int y = c.position.y;
        gf::Vector2i pos(x, y);

        gf::RectangleShape shape({ m_gameData.m_sizeSquare-2.5f, m_gameData.m_sizeSquare -2.5f});
        shape.setPosition(gf::Vector2f(beginPlateau.col+m_gameData.m_sizeSquare/2 + ((float)x * m_gameData.m_sizeSquare), beginPlateau.height+m_gameData.m_sizeSquare/2 + ((float)y * m_gameData.m_sizeSquare)));
        
        // if case selected
        if(pos == m_gameData.m_plateau.coordCaseSelected) {
            
            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
        }else // if my king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() == m_gameData.m_myColor && myTurn && m_gameData.m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else // if adv king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() != m_gameData.m_myColor && !myTurn && m_gameData.m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else if(m_gameData.m_plateau.lastCoup.size() >= 2 && (pos == m_gameData.m_plateau.lastCoup.back() || pos == m_gameData.m_plateau.lastCoup[m_gameData.m_plateau.lastCoup.size()-2])) {

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
        
        target.draw(shape, states);
        
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
                    sprite.setPosition(gf::Vector2f(beginPlateau.col + m_gameData.m_sizeSquare/2 + ((float)x * m_gameData.m_sizeSquare)+(m_gameData.m_sizeSquare/4)*tab[z*2] , beginPlateau.height+m_gameData.m_sizeSquare/2 + ((float)y * m_gameData.m_sizeSquare)+(m_gameData.m_sizeSquare/4)*tab[z*2+1]));
                    sprite.setScale((1.f / (6.5f*2.f)));
                    sprite.setAnchor(gf::Anchor::Center);
                    
                    if (m_gameData.m_myColor == ChessColor::BLACK) {
                        sprite.setRotation(gf::Pi);
                    }

                    target.draw(sprite, states); 
                }
            }else {

                sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(gf::Vector2f(beginPlateau.col + m_gameData.m_sizeSquare/2 + ((float)x * m_gameData.m_sizeSquare) , beginPlateau.height+m_gameData.m_sizeSquare/2 + ((float)y * m_gameData.m_sizeSquare)));
                sprite.setScale((1.f / 6.5f));
                sprite.setAnchor(gf::Anchor::Center);

                if (m_gameData.m_myColor == ChessColor::BLACK) {
                    sprite.setRotation(gf::Pi);
                }

                target.draw(sprite, states);
            }            
        }
   
        // draw move authorized
        auto it = std::find(m_gameData.m_plateau.moveAvailable.begin(), m_gameData.m_plateau.moveAvailable.end(), gf::Vector2i(x, y));
        if(it != m_gameData.m_plateau.moveAvailable.end()) {
        
            gf::CircleShape c(9.f);
            c.setColor(gf::Color::Gray(.6f));
            c.setAnchor(gf::Anchor::Center);
            c.setPosition(gf::Vector2f(beginPlateau.col+m_gameData.m_sizeSquare/2 + ((float)x * m_gameData.m_sizeSquare), beginPlateau.height+m_gameData.m_sizeSquare/2 + ((float)y * m_gameData.m_sizeSquare)));
            target.draw(c, states);
        } 
   }
 
}

gf::Vector2i BoardEntity::getTransformCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    
    gf::Vector2i v(-1,-1); 
    gf::Vector2f beginPlateau(sizeWindows/2-4*m_gameData.m_sizeSquare);
    
    if(true) {
        v.y = (int)((mouseCoord.height-beginPlateau.height)/(m_gameData.m_sizeSquare));
        v.x = (int)((mouseCoord.col-beginPlateau.col)/(m_gameData.m_sizeSquare));
    }
    if(v.y < 0 || v.x < 0) {
        v.x = v.y = -1;
    }

    return v;
}