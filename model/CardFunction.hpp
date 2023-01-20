#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"

void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool NoCardPlayable (Plateau& p, Phase f);

void Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool PrincessIsPlayable (Plateau& p, Phase f);
bool BombeAtomiqueIsPlayable (Plateau& p, Phase f);

#endif
