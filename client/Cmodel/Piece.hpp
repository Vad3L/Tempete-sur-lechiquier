#ifndef PIECE_H
#define PIECE_H

#include <gf/Texture.h>
#include <gf/Sprite.h>
#include <string>
#include <iostream>

#include "./Color.hpp"
#include "./PieceEnum.hpp"

class Piece {


    public:

        /**
        * Build  Piece   
        */
        Piece(Color::);

        /**
        * Guetteur on the color
        */
        Color getColor();

        /**
        * methode to render
        */
        void render(gf::RenderTarget& target);
        
        gf::Texture texture;
        gf::Sprite sprite;

        PieceEnum type;

    private:
        Color color;

        

    
};
#endif // PIECE_H
