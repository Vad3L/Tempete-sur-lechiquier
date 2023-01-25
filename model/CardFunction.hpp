#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"


//tools
bool binNotChange(Plateau& p);
bool isInEchecAfterCardGreen(Plateau &p,std::function<bool(Plateau&, gf::Vector2i s, gf::Vector2i e)> execute);

//card
bool NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool NoCardPlayable (Plateau& p, Phase f);

bool Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool Vampirisme (Plateau& p, gf::Vector2i s = gf::Vector2i(-1), gf::Vector2i e = gf::Vector2i(-1));
bool VisitesOfficielles (Plateau& p, gf::Vector2i s, gf::Vector2i e); 

bool PrincessIsPlayable (Plateau& p, Phase f);
bool BombeAtomiqueIsPlayable (Plateau& p, Phase f);
bool VampirismeIsPlayable (Plateau& p, Phase f);
bool VisitesOfficiellesIsPlayable (Plateau& p, Phase f);

#endif
