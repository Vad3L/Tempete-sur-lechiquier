#include "BoardEntity.hpp"

#include<iostream>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

BoardEntity::BoardEntity(gf::ResourceManager& resources, GameData &gameData)
: m_backgroundTexture(resources.getTexture("ChessSheet.png"))
, m_gameData(gameData)
{
    m_backgroundTexture.setSmooth(true);
}

void BoardEntity::update([[maybe_unused]] gf::Time time) {
}

void BoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);
    gf::Coordinates coords(gf::Vector2i(200.f , 200.f));


    float sizeSquare = coords.getRelativeSize(gf::vec(0.0f, 1.f/8.f)).height;
    float sizeLine = 1.5f;
    bool myTurn = m_gameData.m_myTurn;

    bool promotion = false;
    gf::Vector2f pieceToPromuteCoords;
    ChessColor pieceToPromuteColor;
    
    //draw plateau 
    for  (Case &c : m_gameData.m_plateau.state) {
        int x = c.position.x;
        int y = c.position.y;
        gf::Vector2i pos(x, y);
        
        gf::RectangleShape shape({ sizeSquare-sizeLine, sizeSquare-sizeLine});
        shape.setPosition(coords.getRelativePoint({-0.5f + (float)x * 1.f/8.f + 1/16.f, -0.5f + (float)y * 1.f/8.f + 1/16.f}));
        
        // if case selected
        if(pos == m_gameData.m_plateau.coordCaseSelected) {
            
            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 200));
        }else // if my king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() == m_gameData.m_myColor && myTurn && m_gameData.m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else // if adv king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() != m_gameData.m_myColor && !myTurn && m_gameData.m_plateau.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else if(m_gameData.m_plateau.lastCoup.size() >= 2 && (pos == m_gameData.m_plateau.lastCoup.back() || pos == m_gameData.m_plateau.lastCoup[m_gameData.m_plateau.lastCoup.size()-2])) {

            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 200));
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
        shape.setOutlineThickness(sizeLine);

        target.draw(shape, states);

        // draw pieces
        if (c.piece.getType() != ChessPiece::NONE) {
   
            gf::Sprite sprite;
            float i = (float)c.piece.getType();
            float j = (int)(c.piece.getColor())/4.f;
            
            if(c.piece.getType() == ChessPiece::PAWN && (y == 0 || y == 7) && myTurn) {
                promotion = true;
                pieceToPromuteCoords.x = x;
                pieceToPromuteCoords.y = y;
                pieceToPromuteColor = c.piece.getColor();
            }else {
                
                sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(coords.getRelativePoint({ -0.5f + 1.f/16.f + x * 1.f/8.f, -0.5f + 1.f/16.f + y * 1.f/8.f }));
                sprite.setScale(1.f/14.f);
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
        
            gf::CircleShape circle(4.5f);
            circle.setColor(gf::Color::Gray(.6f));
            circle.setPosition(coords.getRelativePoint({ -0.5f + 1.f/16.f + x * 1.f/8.f, -0.5f + 1.f/16.f + y * 1.f/8.f }));
            circle.setAnchor(gf::Anchor::Center);
            target.draw(circle, states);
        }
    }

    if(promotion){
        gf::RectangleShape rectangle({sizeSquare-sizeLine,((sizeSquare*4)-sizeLine)});
        rectangle.setColor(gf::Color::White);
        rectangle.setOutlineColor(gf::Color::Black);
        rectangle.setOutlineThickness(sizeLine);
        
        /*std::cout << "piece y :" << (float)pieceToPromuteCoords.y * 1.f/8.f + 1/16.f << std::endl;
        std::cout << "sizeSquare :" << sizeSquare << std::endl;*/

        int mul = (pieceToPromuteColor == ChessColor::WHITE) ? 1 : -1;

        rectangle.setPosition(coords.getRelativePoint({-0.5f + (float)pieceToPromuteCoords.x * 1.f/8.f + 1/16.f, -0.5f + (float)pieceToPromuteCoords.y * 1.f/8.f + (pieceToPromuteColor == ChessColor::WHITE ? (1/4.f) : (-1/8.f))}));
        rectangle.setAnchor(gf::Anchor::Center);
        target.draw(rectangle, states);
        
        for(int z = 0; z < 4 ; z++) {
            //std::cout << << std::endl;
            gf::Sprite sprite;
            sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * (z+1), (int)(pieceToPromuteColor)/4.f }, { (1.f / numberPiece), 0.25f }));
            sprite.setPosition(coords.getRelativePoint({ (float)pieceToPromuteCoords.x * 1.f/8.f + -0.5f + 1.f/16.f, (float)(pieceToPromuteCoords.y+z)* 1.f/8.f + -0.5f + (pieceToPromuteColor == ChessColor::WHITE ? (1/16.f) : (-1/3.2f))}));
            sprite.setScale(1.f/14.f);
            sprite.setAnchor(gf::Anchor::Center);
            
            if (m_gameData.m_myColor == ChessColor::BLACK) {
                sprite.setRotation(gf::Pi);
            }
            target.draw(sprite, states);
        }
    }
}

gf::Vector2i BoardEntity::getTransformCaseSelected(gf::Vector2i sizeWindows, gf::Vector2i mouseCoord) {
    
    gf::Vector2i v(-1,-1); 
    float min = std::min(sizeWindows.height, sizeWindows.width);
    
    v.y = (int)((8.f/min) * (mouseCoord.y + min/2.f)); //+min/2 pour ramener les coordonées dans le positif 
    v.x = (int)((8.f/min) * (mouseCoord.x + min/2.f));
    
    if(v.y < 0 || v.x < 0 || v.y > 7 || v.x > 7) {
        v.x = v.y = -1;
    }

    std::cout << "caseSelectionne: [ligne/col] " << v.y << "," << v.x << std::endl;
    return v;
}

ChessPiece BoardEntity::getChoice(gf::Vector2i sizeWindows, gf::Vector2i clickCoord) {
    ChessPiece p;
    std::cout << "case cliquer " << clickCoord.y<< ","<< clickCoord.x<< std::endl;
    float min = std::min(sizeWindows.height, sizeWindows.width);
        
    if(m_gameData.m_myColor == ChessColor::WHITE){
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
    }else{
        switch(clickCoord.y){
            case 7:
                p = ChessPiece::ROOK;
                break;
            case 6:
                p = ChessPiece::KNIGHT;
                break;
            case 5:
                p = ChessPiece::BISHOP;
                break;
            case 4:
                p = ChessPiece::QUEEN;
                break;
        }
    }
    
    return p;
}