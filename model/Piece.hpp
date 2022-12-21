#ifndef PIECE_H
#define PIECE_H

#include <gf/Texture.h>
#include <gf/Sprite.h>
#include <string>
#include <iostream>

class Piece {


    public:

        /**
        * Build  Piece   
        */
        Piece(std::string path);
        gf::Texture texture;
        gf::Sprite sprite;

    private:

        

    
};
#endif // PIECE_H
