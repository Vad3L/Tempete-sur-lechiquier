#include "TableBoardEntity.hpp"

#include<iostream>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

TableBoardEntity::TableBoardEntity(gf::ResourceManager& resources, GameData &gameData)
: m_font(resources.getFont("DejaVuSans.ttf"))
, m_backgroundTexture(resources.getTexture("ChessSheet.png"))
,m_gameData(gameData)
{
    m_backgroundTexture.setSmooth(true);
}

void TableBoardEntity::update([[maybe_unused]] gf::Time time) {
}

void TableBoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    gf::Vector2f beginPlateau(coords.getWindowSize()/2-4*m_gameData.m_sizeSquare);
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);

    gf::RectangleShape tableCloth(gf::Vector2f(672.f, 560.f));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setPosition(coords.getWindowSize()/2);
    tableCloth.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
    target.draw(tableCloth, states);


    std::string turn = (m_gameData.m_myTurn) ? std::string("It\'s your turn !") : std::string("It's opponent's turn !");
    gf::Text text(turn, m_font, 20);
    
    text.setPosition(gf::Vector2f(coords.getWindowSize().x/2, coords.getWindowSize().y/2-250.f));
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::Black);
    target.draw(text, states);

    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    int incr = (m_gameData.m_myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], m_font, 20);
        
        text.setPosition(gf::Vector2f(beginPlateau.col-m_gameData.m_sizeSquare/2 + (i*m_gameData.m_sizeSquare), (beginPlateau.height)+(8.5*m_gameData.m_sizeSquare)));
        text.setAnchor(gf::Anchor::Center);
        text.setColor(gf::Color::fromRgba32(238,198,108));
        target.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(gf::Vector2f(beginPlateau.col - m_gameData.m_sizeSquare/2, (beginPlateau.height - m_gameData.m_sizeSquare/2)+(i*m_gameData.m_sizeSquare)));
        
        target.draw(text, states);
        (m_gameData.m_myColor == ChessColor::WHITE) ? incr-- : incr++; 
    }

    // draw bin
    int tabW[numberPiece];
    int tabB[numberPiece];
    for(int i = 0 ; i < numberPiece ; ++i){
        tabB[i] = 0;
        tabW[i] = 0;
    }
    
    for(Piece &pi : m_gameData.m_plateau.bin) {
        gf::Sprite sprite;
        float i = (float)pi.getType();
		float j = (int)(pi.getColor())/4.f;
		
        sprite.setTexture(m_backgroundTexture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));

        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(gf::Vector2f(beginPlateau.col-(tabW[(int)i]%4)*15 + (-1.25f * m_gameData.m_sizeSquare) , beginPlateau.height + (tabW[(int)i]/4)*15 + (i * m_gameData.m_sizeSquare)));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(gf::Vector2f(beginPlateau.col + (tabB[(int)i]%4)*15 + (8.25f * m_gameData.m_sizeSquare) , beginPlateau.height + (tabB[(int)i]/4)*15 + (i * m_gameData.m_sizeSquare)));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 8.f));
        target.draw(sprite, states);
    } 
}
