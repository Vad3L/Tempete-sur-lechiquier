#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, ChessColor mycolor) : window("Tempete sur l'échiquier", SSize),
						renderer(window),
						myColor(mycolor) {

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
    sheetNumberLetter = gf::Texture("../data/NumberLetterSheet.png");
}

void Vue::draw(Plateau p) {
    
    renderer.setView(mainView);

    gf::RectangleShape tableCloth(boardSize + 2 * sizeSquare);
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setColor(gf::Color::fromRgba32(85,60,40));
    tableCloth.setPosition(ScreenSize/2);
    renderer.draw(tableCloth);

    //print border of board with letter and number
    for(int i = 1 ; i <= 8;++i) {
        gf::Sprite sprite;
        // left numbers
        sprite.setTexture(sheetNumberLetter, gf::RectF::fromPositionSize({ (1.f / 8.f) * (8-i), 0.f }, { (1.f / 8.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginBoard.col - sizeSquare/2,(beginBoard.height - sizeSquare/2)+(i*sizeSquare)));
        sprite.setScale((1.f / 20.f));
        sprite.setAnchor(gf::Anchor::Center);
        renderer.draw(sprite);
        
        // right numbers
        sprite.setTexture(sheetNumberLetter, gf::RectF::fromPositionSize({ (1.f / 8.f) * (i-1), 0.f }, { (1.f / 8.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginBoard.col+ (sizeSquare *8.5),(beginBoard.height - sizeSquare/2)+(i*sizeSquare)));
        sprite.setRotation(gf::Pi);
        renderer.draw(sprite);
    }

    for(int i = 1 ; i <= 8;++i){
        gf::Sprite sprite;
        // bottom letters
        sprite.setTexture(sheetNumberLetter, gf::RectF::fromPositionSize({ (1.f / 8.f) * (i-1), 0.5f }, { (1.f / 8.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginBoard.col-sizeSquare/2 + (i*sizeSquare),(beginBoard.height)+(8.5*sizeSquare)));
        sprite.setScale((1.f / 22.f));
        sprite.setAnchor(gf::Anchor::Center);
        renderer.draw(sprite);

        // top letters
        sprite.setTexture(sheetNumberLetter, gf::RectF::fromPositionSize({ (1.f / 8.f) * (8-i), 0.5f }, { (1.f / 8.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginBoard.col-sizeSquare/2 + (i*sizeSquare),(beginBoard.height - sizeSquare/2)));
        sprite.setRotation(gf::Pi);
        renderer.draw(sprite);  
    }

    gf::Color4f lightBrown = gf::Color::fromRgba32(240,217,181,255);
    gf::Color4f darkBrown = gf::Color::fromRgba32(181,136,99,255);

    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    //shape.setAnchor(gf::Anchor::Center);
   
    //affichage plateau 
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

        if(y == p.caseSelected.y && x == p.caseSelected.x) {
            shape.setColor(gf::Color::Red);
        }
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