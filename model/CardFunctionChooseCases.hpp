#ifndef CARD_FUNC_CHOOSE_CASES_H
#define CARD_FUNC_CHOOSE_CASES_H

#include "Plateau.hpp"

void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Exil(Plateau& p, gf::Vector2i s, gf::Vector2i e);
void ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ExilIsPlayable(Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);

#endif // CARD_FUNC_CHOOSE_CASES_H
