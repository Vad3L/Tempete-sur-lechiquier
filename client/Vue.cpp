#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, ChessColor mycolor) : window("Tempete sur l'échiquier", SSize),
						renderer(window),
						myColor(mycolor),
                        font("../data/DejaVuSans.ttf") {

    ScreenSize = gf::Vector2u(1300, 1000);
    sizeSquare = 64.0f;
    beginPlateau = gf::Vector2f(394.0f, 244.0f);
    plateauSize = gf::Vector2f(sizeSquare * 8, sizeSquare * 8);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    plateauView = gf::LockedView(ScreenSize / 2, gf::Vector2f(sizeSquare*8));
    boardView = gf::LockedView(ScreenSize/2, gf::Vector2f(sizeSquare*12, sizeSquare * 10));

    views.addView(plateauView);
    views.addView(boardView);
    views.addView(screenView);
    views.setInitialFramebufferSize(ScreenSize);

    sheetPiece = gf::Texture("../data/ChessSheet.png");
}

void Vue::draw(Plateau p) {
    
    renderer.setView(boardView);

    gf::RectangleShape tableCloth(gf::Vector2f(plateauSize.x + 4 * sizeSquare, plateauSize.y + 2 * sizeSquare));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setColor(gf::Color::fromRgba32(85,60,40));
    tableCloth.setPosition(ScreenSize/2);
    renderer.draw(tableCloth);
    
    std::string letters[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    int incr = (myColor == ChessColor::WHITE) ? 7 : 0; 
    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) { 
        // letter
        gf::Text text(letters[7 - incr], font, 20);
        
        text.setPosition(gf::Vector2f(beginPlateau.col-sizeSquare/2 + (i*sizeSquare), (beginPlateau.height)+(8.5*sizeSquare)));
        text.setAnchor(gf::Anchor::Center);
        renderer.draw(text);

        // number   
        text.setString(std::to_string(incr+1));
        text.setPosition(gf::Vector2f(beginPlateau.col - sizeSquare/2, (beginPlateau.height - sizeSquare/2)+(i*sizeSquare)));
        
        renderer.draw(text);
        (myColor == ChessColor::WHITE) ? incr-- : incr++; 
    }
    

    int tabW[6] = {0, 0, 0, 0, 0, 0};
    int tabB[6] = {0, 0, 0, 0, 0, 0};
    for(Piece &pi : p.bin) {
        gf::Sprite sprite;
        float i = (float)pi.getType();
		float j = (pi.getColor() == ChessColor::WHITE) ? 0 : 0.5f;
		
        sprite.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / 6.f) * i, j }, { (1.f / 6.f), 0.5f }));

        if(pi.getColor() == ChessColor::WHITE) {
            sprite.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2-(tabW[(int)i]%4)*15 + ((float)-1.5 * sizeSquare) , beginPlateau.height+sizeSquare/2 + (tabW[(int)i]/4)*15 + ((float)i * sizeSquare)));
            tabW[(int)i]+=1;
        }else {
            sprite.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2+(tabB[(int)i]%4)*15 + ((float)8 * sizeSquare) , beginPlateau.height+sizeSquare/2 + (tabB[(int)i]/4)*15 + ((float)i * sizeSquare)));
            tabB[(int)i]+=1;
        }
        
        sprite.setScale((1.f / 6.f));
        sprite.setAnchor(gf::Anchor::Center);
        renderer.draw(sprite);
    }
    
    renderer.setView(plateauView);

    gf::Color4f lightBrown = gf::Color::fromRgba32(240,217,181,255);
    gf::Color4f darkBrown = gf::Color::fromRgba32(181,136,99,255);

    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    //shape.setAnchor(gf::Anchor::Center);
    
    //draw plateau 
    for  (Case &c : p.state) {
        int x = c.position.x;
        int y = c.position.y;
        shape.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2 + ((float)x * sizeSquare), beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)));
        
        if (y % 2 == 0) {
            if (x % 2 == 0) {
                shape.setColor(lightBrown);
            }
            else {
                shape.setColor(darkBrown);
            }
        } else {
            if (x % 2 == 0) {
                shape.setColor(darkBrown);
            }
            else {
                shape.setColor(lightBrown);
            }
        }

        // case selected
        if(y == p.coordCaseSelected.y && x == p.coordCaseSelected.x) {
            shape.setColor(gf::Color::Red);
        }

        // move authorized
        for(auto &coord : p.moveAvailable) {
            if(y == coord.y && x == coord.x) {
                shape.setColor(gf::Color::Gray(0.6));
                break;
            }
        }

        shape.setAnchor(gf::Anchor::Center);
        shape.setOutlineColor(gf::Color::fromRgba32(85,60,40));
        shape.setOutlineThickness(2.5f);
        renderer.draw(shape);

	if (c.piece.getType() != ChessPiece::NONE) {
		gf::Sprite sprite;
		float i = (float)c.piece.getType();
		float j = (c.piece.getColor() == ChessColor::WHITE) ? 0 : 0.5f;
		
        sprite.setTexture(sheetPiece, gf::RectF::fromPositionSize({ (1.f / 6.f) * i, j }, { (1.f / 6.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginPlateau.col+sizeSquare/2 + ((float)x * sizeSquare) , beginPlateau.height+sizeSquare/2 + ((float)y * sizeSquare)));
        sprite.setScale((1.f / 6.f));
        sprite.setAnchor(gf::Anchor::Center);
		
        if (myColor == ChessColor::BLACK) {
            sprite.setRotation(gf::Pi);
        }
        
		renderer.draw(sprite);
	}
   } 

   renderer.setView(screenView);
   
}

void Vue::setColor (ChessColor color) {
    myColor = color;
    plateauView.setRotation(0);
    
    std::string t = (myColor == ChessColor::WHITE) ? "White" : "Black";
    window.setTitle("Tempete sur l'échiquier - " + t);
    
    if (myColor == ChessColor::BLACK) {
	    plateauView.setRotation(gf::Pi);
    } 
}

gf::Vector2i Vue::transformInSelectedCase(gf::Vector2i mouseCoord) {
    gf::Vector2i mouseCoords = renderer.mapPixelToCoords(mouseCoord, plateauView);
    //std::cout << "la : " << ((mouseCoords.height-beginPlateau.col))<< ";" << (mouseCoords.col-beginPlateau.height) << std::endl;
    gf::Vector2i v(-1,-1); 

    if(mouseCoords.height > beginPlateau.height && mouseCoords.height < beginPlateau.height+plateauSize.height && mouseCoords.col > beginPlateau.col && mouseCoords.col < beginPlateau.col+plateauSize.col) {
        v.y = (int)((mouseCoords.height-beginPlateau.height)/(sizeSquare));
        v.x = (int)((mouseCoords.col-beginPlateau.col)/(sizeSquare));
    }

    std::cout << "caseSelectionne: [ligne/col] " << v.y << "," << v.x << std::endl;
    return v;
}
