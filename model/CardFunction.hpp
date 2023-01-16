#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"

void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool NoCardPlayable (Plateau& p, Phase f);

void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Exil(Plateau& p, gf::Vector2i s, gf::Vector2i e);

bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ExilIsPlayable(Plateau& p, Phase f);

#endif
