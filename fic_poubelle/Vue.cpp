#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, ChessColor mycolor) : window("Tempete sur l'échiquier", SSize),
						renderer(window),
						myColor(mycolor),
                        font("../data/DroidSans.ttf") {

    ScreenSize = gf::Vector2u(1300, 1000);
    sizeSquare = 56.f;
    beginPlateau = gf::Vector2f(426.f, 276.f);
    plateauSize = gf::Vector2f(sizeSquare * 8+5.f, sizeSquare * 8+5.f);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    boardView = gf::LockedView(ScreenSize/2, plateauSize);
    gameView = gf::LockedView(ScreenSize/2, gf::Vector2f(sizeSquare*12, sizeSquare*10));

    views.addView(boardView);
    views.addView(gameView);
    views.addView(screenView);
    views.setInitialFramebufferSize(ScreenSize);

    sheetPiece = gf::Texture("../data/ChessSheet.png");
}

void Vue::draw(Plateau p, bool myTurn) {
    
    int numberPiece = ((int)ChessPiece::MAX - (int)ChessPiece::MIN + 1);

    renderer.setView(gameView);

    gf::RectangleShape tableCloth(gf::Vector2f(plateauSize.x + 4 * sizeSquare, plateauSize.y + 2 * sizeSquare));
    tableCloth.setAnchor(gf::Anchor::Center);
    //tableCloth.setColor(gf::Color::fromRgba32(85,60,40));
    tableCloth.setPosition(ScreenSize/2);
    tableCloth.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 2, .75f }, { (1.f / numberPiece), 0.25f }));
    renderer.draw(tableCloth);
    
    std::string turn = (myTurn) ? std::string("It\'s your turn !") : std::string("It's opponent's turn !");
    gf::Text text(turn, font, 20);
    
    text.setPosition(gf::Vector2f(ScreenSize.x/2.f, (beginPlateau.height)-25.f));
    text.setAnchor(gf::Anchor::Center);
    text.setColor(gf::Color::Black);
    renderer.draw(text);

    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    int incr = (myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], font, 20);
        
        text.setPosition(gf::Vector2f(beginPlateau.col-sizeSquare/2 + (i*sizeSquare), (beginPlateau.height)+(8.5*sizeSquare)));
        text.setAnchor(gf::Anchor::Center);
        text.setColor(gf::Color::fromRgba32(238,198,108));
        renderer.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(gf::Vector2f(beginPlateau.col - sizeSquare/2, (beginPlateau.height - sizeSquare/2)+(i*sizeSquare)));
        
        renderer.draw(text);
        (myColor == ChessColor::WHITE) ? incr-- : incr++; 
    }
    
    // draw bin
    int tabW[numberPiece];
    int tabB[numberPiece];
    for(int i = 0 ; i < numberPiece ; ++i){
        tabB[i] = 0;
        tabW[i] = 0;
    }
    
    for(Piece &pi : p.bin) {
        gf::Sprite sprite;
        float i = (float)pi.getType();
		float j = (int)(pi.getColor())/4.f;
		
        sprite.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));

        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(gf::Vector2f(beginPlateau.col-(tabW[(int)i]%4)*15 + (-1.25f * sizeSquare) , beginPlateau.height + (tabW[(int)i]/4)*15 + (i * sizeSquare)));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(gf::Vector2f(beginPlateau.col + (tabB[(int)i]%4)*15 + (8.25f * sizeSquare) , beginPlateau.height + (tabB[(int)i]/4)*15 + (i * sizeSquare)));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 8.f));
        renderer.draw(sprite);
    }
    
    renderer.setView(boardView);
    
    //draw plateau 
    for  (Case &c : p.state) {
        int x = c.position.x;
        int y = c.position.y;
        gf::Vector2i pos(x, y);

        gf::RectangleShape shape({ sizeSquare-2.5f, sizeSquare -2.5f});
        shape.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2 + ((float)x * sizeSquare), beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)));
        
        // if case selected
        if(pos == p.coordCaseSelected) {
            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
            //shape.setColor(gf::Color::fromRgba32(30, 144, 255, 180));
        }else // if my king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() == myColor && myTurn && p.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else // if adv king is in echec
        if(c.piece.getType() == ChessPiece::KING && c.piece.getColor() != myColor && !myTurn && p.playerInEchec) {
            
            shape.setColor(gf::Color::fromRgba32(255, 0, 0, 100));
        }else if(p.lastCoup.size() >= 2 && (pos == p.lastCoup.back() || pos == p.lastCoup[p.lastCoup.size()-2])) {

            shape.setColor(gf::Color::fromRgba32(250, 190, 88, 160));
        }else {
            
            if (y % 2 == 0) {
                if (x % 2 == 0) {
                    shape.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f}));
                }
                else {
                    shape.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
            } else {
                if (x % 2 == 0) {
                    shape.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 0, .75f }, { (1.f / numberPiece), .5f }));
                }
                else {
                    shape.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * 1, .75f }, { (1.f / numberPiece), .5f }));
                }
            }
        }

        
        shape.setAnchor(gf::Anchor::Center);
        shape.setOutlineColor(gf::Color::fromRgba32(85,60,40));
        shape.setOutlineThickness(2.5f);
        
        renderer.draw(shape);
        
        // draw piece
        if (c.piece.getType() != ChessPiece::NONE) {
            gf::Sprite sprite;
            float i = (float)c.piece.getType();
            float j = (int)(c.piece.getColor())/4.f;
            
            if(c.piece.getType() == ChessPiece::PAWN && (y == 0 || y == 7)) {
                
                int mul = (c.piece.getColor() == ChessColor::WHITE) ? 1: -1;
                int tab[8] = {-1*mul,-1*mul,-1*mul, 1*mul, 1*mul, 1*mul, 1*mul, -1*mul};
                for(int z = 0; z < 4 ; z++) {
                    sprite.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * (z+1), j }, { (1.f / numberPiece), 0.25f }));
                    sprite.setPosition(gf::Vector2f(beginPlateau.col + sizeSquare/2 + ((float)x * sizeSquare)+(sizeSquare/4)*tab[z*2] , beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)+(sizeSquare/4)*tab[z*2+1]));
                    sprite.setScale((1.f / (6.5f*2.f)));
                    sprite.setAnchor(gf::Anchor::Center);
                    
                    if (myColor == ChessColor::BLACK) {
                        sprite.setRotation(gf::Pi);
                    }

                    renderer.draw(sprite); 
                }
            }else {

                sprite.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / numberPiece) * i, j }, { (1.f / numberPiece), 0.25f }));
                sprite.setPosition(gf::Vector2f(beginPlateau.col + sizeSquare/2 + ((float)x * sizeSquare) , beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)));
                sprite.setScale((1.f / 6.5f));
                sprite.setAnchor(gf::Anchor::Center);
                
                if (myColor == ChessColor::BLACK) {
                    sprite.setRotation(gf::Pi);
                }

                renderer.draw(sprite);
            }            
        }
   
        // draw move authorized
        auto it = std::find(p.moveAvailable.begin(), p.moveAvailable.end(), gf::Vector2i(x, y));
        if(it != p.moveAvailable.end()) {
        
            gf::CircleShape c(9.f);
            c.setColor(gf::Color::Gray(.6f));
            c.setAnchor(gf::Anchor::Center);
            c.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2 + ((float)x * sizeSquare), beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)));
            renderer.draw(c);
        } 
   } 
   
   renderer.setView(screenView);
   
}

void Vue::setColor (ChessColor color) {
    myColor = color;
    boardView.setRotation(0);
    
    std::string t = (myColor == ChessColor::WHITE) ? "White" : "Black";
    window.setTitle("Tempete sur l'échiquier - " + t);
    
    if (myColor == ChessColor::BLACK) {
	    boardView.setRotation(gf::Pi);
    } 
}

gf::Vector2i Vue::transformInSelectedCase(gf::Vector2i mouseCoord) {
    gf::Vector2i mouseCoords = renderer.mapPixelToCoords(mouseCoord, boardView);
    //std::cout << "la : " << ((mouseCoords.height-beginPlateau.height))<< ";" << (mouseCoords.col-beginPlateau.col) << std::endl;
    gf::Vector2i v(-1,-1); 

    if(mouseCoords.height > beginPlateau.height && mouseCoords.height < beginPlateau.height+plateauSize.height && mouseCoords.col > beginPlateau.col && mouseCoords.col < beginPlateau.col+plateauSize.col) {
        v.y = (int)((mouseCoords.height-beginPlateau.height)/(sizeSquare));
        v.x = (int)((mouseCoords.col-beginPlateau.col)/(sizeSquare));
    }

    std::cout << "caseSelectionne: [ligne/col] " << v.y << "," << v.x << std::endl;
    return v;
}