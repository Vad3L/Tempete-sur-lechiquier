#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, ChessColor mycolor) : window("Tempete sur l'échiquier", SSize),
						renderer(window),
						myColor(mycolor),
                        font("../data/DejaVuSans.ttf") {

    ScreenSize = gf::Vector2u(1300, 1000);
    sizeSquare = 64.0f;
    beginBoard = gf::Vector2f(394.0f, 244.0f);
    boardSize = gf::Vector2f(sizeSquare * 8, sizeSquare * 8);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    mainView = gf::LockedView(ScreenSize / 2, gf::Vector2f(sizeSquare*10));
    
    views.addView(mainView);
    views.addView(screenView);
    views.setInitialFramebufferSize(ScreenSize);

    sheetPiece = gf::Texture("../data/ChessSheet.png");
}

void Vue::draw(Plateau p) {
    
    renderer.setView(mainView);

    gf::RectangleShape tableCloth(boardSize + 2 * sizeSquare);
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setColor(gf::Color::fromRgba32(85,60,40));
    tableCloth.setPosition(ScreenSize/2);
    renderer.draw(tableCloth);
    
    std::string tab[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};

    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i){ 
        // letter
        float translation = (myColor == ChessColor::BLACK) ? 9*sizeSquare : 0;
        gf::Text text(tab[i-1], font, 20);
        text.setPosition(gf::Vector2f(beginBoard.col-sizeSquare/2 + (i*sizeSquare),(beginBoard.height)+(8.5*sizeSquare) - translation));
        
        if (myColor == ChessColor::BLACK) {
            text.setRotation(gf::Pi);
        }
        
        text.setAnchor(gf::Anchor::Center);
        renderer.draw(text);

        // number   
        text.setString(std::to_string(9-i));
        text.setPosition(gf::Vector2f(beginBoard.col - sizeSquare/2  + translation,(beginBoard.height - sizeSquare/2)+(i*sizeSquare)));
        
        renderer.draw(text);
    }

    gf::Color4f lightBrown = gf::Color::fromRgba32(240,217,181,255);
    gf::Color4f darkBrown = gf::Color::fromRgba32(181,136,99,255);

    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    //shape.setAnchor(gf::Anchor::Center);
    
    //draw plateau 
    for  (Case &c : p.state) {
        int x = c.position.x;
        int y = c.position.y;
        shape.setPosition(gf::Vector2f(beginBoard.col+sizeSquare/2 + ((float)x * sizeSquare), beginBoard.height+sizeSquare/2 + ((float)y * sizeSquare)));
        
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
        if(y == p.caseSelected.y && x == p.caseSelected.x) {
            shape.setColor(gf::Color::Red);
        }

        // move autorized
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
        	sprite.setPosition(gf::Vector2f(beginBoard.col+sizeSquare/2 + ((float)x * sizeSquare) , beginBoard.height+sizeSquare/2 + ((float)y * sizeSquare)));
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
    mainView.setRotation(0);
    
    std::string t = (myColor == ChessColor::WHITE) ? "White" : "Black";
    window.setTitle("Tempete sur l'échiquier - " + t);
    
    if (myColor == ChessColor::BLACK) {
	    mainView.setRotation(gf::Pi);
    } 
}

gf::Vector2i Vue::transformInSelectedCase(gf::Vector2i mouseCoord) {
    gf::Vector2i mouseCoords = renderer.mapPixelToCoords(mouseCoord, mainView);
    //std::cout << "la : " << ((mouseCoords.height-beginBoard.col))<< ";" << (mouseCoords.col-beginBoard.height) << std::endl;
    gf::Vector2i v(-1,-1); 

    if(mouseCoords.height > beginBoard.height && mouseCoords.height < beginBoard.height+boardSize.height && mouseCoords.col > beginBoard.col && mouseCoords.col < beginBoard.col+boardSize.col) {
        v.y = (int)((mouseCoords.height-beginBoard.height)/(sizeSquare));
        v.x = (int)((mouseCoords.col-beginBoard.col)/(sizeSquare));
    }

    std::cout << "caseSelectionne: [ligne/col] " << v.y << "," << v.x << std::endl;
    return v;
}
