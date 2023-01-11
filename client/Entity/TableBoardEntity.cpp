#include "TableBoardEntity.hpp"

#include<iostream>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

TableBoardEntity::TableBoardEntity(gf::ResourceManager& resources, GameData &gameData)
: m_font(resources.getFont("fonts/DejaVuSans.ttf"))
, m_backgroundTexture(resources.getTexture("images/ChessSheet.png"))
, m_backgroundTexture2(resources.getTexture("images/ChessSheet2.png"))
,m_gameData(gameData)
{
    m_backgroundTexture.setSmooth(true);
    m_backgroundTexture2.setSmooth(true);
}

void TableBoardEntity::update([[maybe_unused]] gf::Time time) {
}

void TableBoardEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(gf::Vector2i(450.f , 450.f));

    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);
    gf::Texture &texture = (m_gameData.m_style == 0) ? m_backgroundTexture : m_backgroundTexture2;

    gf::RectangleShape tableCloth(gf::Vector2f(500.f, 400.f));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setPosition(coords.getRelativePoint({0.f, 0.f}));
    tableCloth.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
    target.draw(tableCloth,states);

    
    std::string indication = (m_gameData.m_myTurn) ? std::string("It's your turn !") : std::string("It's opponent's turn !");
    
    switch (m_gameData.m_gameStatus) {
        case ChessStatus::WIN:
            indication = "You WIN";
            break;
        case ChessStatus::LOOSE:
            indication = "You LOOSE";
            break;
        case ChessStatus::EQUALITY:
            indication = "There is equality";
            break;
        case ChessStatus::NO_STARTED:
            indication = "Game no started";
            break;
    }
    

    gf::Text text(indication, m_font);
    text.setScale(0.5f);
    text.setPosition(gf::Vector2f(coords.getRelativePoint({0.f, -0.4f})));
    text.setAnchor(gf::Anchor::Center);
    (m_gameData.m_style==0) ? text.setColor(gf::Color::Black) : text.setColor(gf::Color::Gray(0.8f)) ;
    target.draw(text, states);

    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    
    int incr = (m_gameData.m_myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], m_font);
        
        text.setScale(0.5f);
        text.setPosition({coords.getRelativePoint({-0.315f + (i-1) * 0.086f, 0.4f})});
        
        if(m_gameData.m_style == 0) {
            text.setColor(gf::Color::fromRgba32(238,198,108));
        }else {
            text.setColor(gf::Color::fromRgba32(160,160,160));
        }
        
        target.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(coords.getRelativePoint({-0.39f, -0.29f + (i-1) * 0.086f}));
        
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
		
        sprite.setTexture(texture, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
        
        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(coords.getRelativePoint({-0.45f - 0.03f * (tabW[(int)i]%4), -0.24f + (i-1) * 0.082f + 0.03f * (tabW[(int)i]/4)}));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(coords.getRelativePoint({0.38f + 0.03f * (tabB[(int)i]%4), -0.24f + (i-1) * 0.082f + 0.03f * (tabB[(int)i]/4)}));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 14.f));
        target.draw(sprite, states);
    } 
    
}