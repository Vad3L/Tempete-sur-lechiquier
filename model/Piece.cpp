#include "Piece.hpp"
#include <gf/Image.h>

Piece::Piece(std::string path){
    gf::Image img(path);

    this->texture = gf::Texture(img);
    this->texture.setSmooth(true);
    this->texture.generateMipmap();
    
    this->sprite.setTexture(this->texture);
    std::cout << "piece created\n";
}