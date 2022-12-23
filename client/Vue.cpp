#include "Vue.hpp"

Vue::Vue(gf::Vector2u SSize, int mycolor) : window("tempete sur l'échiquier", SSize),
						renderer(window),
						myColor(mycolor) {
    ScreenSize = gf::Vector2u(1000, 1000);
    sizeSquare = 50.0f;
    beginBoard = gf::Vector2f(300.0f, 300.0f);
    gf::Vector2f boardSize(sizeSquare * 10.0f, sizeSquare * 10.0f);

    window.setSize(ScreenSize);
    window.setFramerateLimit(60);


    //screenView
    mainView = gf::LockedView(ScreenSize / 2, boardSize + 50.0f);

    views.addView(mainView);
    views.addView(screenView);
    views.setInitialFramebufferSize(ScreenSize);

    sheet = gf::Texture("../data/Piece/ChessSheet.png");
}
void Vue::print(Plateau p) {
    
    renderer.setView(mainView);

    gf::Color4f lightBrown = gf::Color::fromRgba32(240,217,181,255);
    gf::Color4f darkBrown = gf::Color::fromRgba32(181,136,99,255);

    gf::RectangleShape shape({ sizeSquare, sizeSquare });
    shape.setAnchor(gf::Anchor::Center);
    
    for(int i = 0 ; i < 10;++i){
        shape.setPosition(gf::Vector2f(beginBoard.height - sizeSquare,(beginBoard.col - sizeSquare)+(i*sizeSquare)));
        shape.setColor(gf::Color::fromRgba32(85,60,40));
        renderer.draw(shape);
    }
    
    for(int i = 0 ; i < 10;++i){
        shape.setPosition(gf::Vector2f(beginBoard.height + (sizeSquare *8),(beginBoard.col - sizeSquare)+(i*sizeSquare)));
        shape.setColor(gf::Color::fromRgba32(85,60,40));
        renderer.draw(shape);
    }

    for(int i = 0 ; i < 8;++i){
        shape.setPosition(gf::Vector2f(beginBoard.height + (i*sizeSquare),(beginBoard.col - sizeSquare)));
        shape.setColor(gf::Color::fromRgba32(85,60,40));
        renderer.draw(shape);
    }

    for(int i = 0 ; i < 8;++i){
        shape.setPosition(gf::Vector2f(beginBoard.height + (i*sizeSquare),(beginBoard.col)+(8*sizeSquare)));
        shape.setColor(gf::Color::fromRgba32(85,60,40));
        renderer.draw(shape);
    }

    //affichage plateau 
    for  (Case &c : p.state) {
        int x = c.position.x;
        int y = c.position.y;

        shape.setPosition(gf::Vector2f(beginBoard.height + ((float)x * sizeSquare), beginBoard.col + ((float)y * sizeSquare)));
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
        renderer.draw(shape);

	if (c.piece.getType() != NONE) {
		gf::Sprite sprite;
		float i = 0;
		float j = (c.piece.getColor() == WHITE) ? 0 : 0.5f;
		switch (c.piece.getType()) {
			case PAWN:
				i = 5;
				break;
			case KNIGHT:
				i = 3;
				break;
			case BISHOP:
				i = 2;
				break;
			case ROOK:  
				i = 4;
				break;
			case QUEEN:
				i = 1;
				break;
			case KING:
				i = 0;
				break;
		}
        
        sprite.setTexture(sheet, gf::RectF::fromPositionSize({ (1.f / 6.f) * i, j }, { (1.f / 6.f), 0.5f }));
        sprite.setPosition(gf::Vector2f(beginBoard.height + ((float)x * sizeSquare) , beginBoard.col + ((float)y * sizeSquare)));
        //sprite.setColor(gf::Color::Black);
        sprite.setScale((1.f / 6.f));
        sprite.setAnchor(gf::Anchor::Center);
		
        if (myColor == -1) {
            sprite.setRotation(gf::Pi);
        }

        renderer.draw(sprite);
	}
   }

    renderer.setView(screenView);
    
}

void Vue::set_side (int color) {
    myColor = color;
    if (myColor == -1) {
	    mainView.setRotation(gf::Pi);
    } 
}

