#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"

void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool NoCardPlayable (Plateau& p, Phase f);

void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Exil(Plateau& p, gf::Vector2i s, gf::Vector2i e);
void BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool ChameauIsPlayable (Plateau& p, Phase f);
bool PrincessIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ExilIsPlayable(Plateau& p, Phase f);
bool BombeAtomiqueIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);

#endif
