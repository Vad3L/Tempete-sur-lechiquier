#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"


//tools
bool binNotChange(Plateau& p);
bool isInEchecAfterCard(Plateau &p,std::function<void(Plateau&, gf::Vector2i s, gf::Vector2i e)> execute);

//card
void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool NoCardPlayable (Plateau& p, Phase f);

void Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Vampirisme (Plateau& p, gf::Vector2i s = gf::Vector2i(-1), gf::Vector2i e = gf::Vector2i(-1));
void VisitesOfficielles (Plateau& p, gf::Vector2i s, gf::Vector2i e); 

bool PrincessIsPlayable (Plateau& p, Phase f);
bool BombeAtomiqueIsPlayable (Plateau& p, Phase f);
bool VampirismeIsPlayable (Plateau& p, Phase f);
bool VisitesOfficiellesIsPlayable (Plateau& p, Phase f);




#endif
