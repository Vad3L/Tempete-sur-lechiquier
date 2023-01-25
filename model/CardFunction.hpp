#ifndef CARD_FUNC_H
#define CARD_FUNC_H

#include "Plateau.hpp"


//tools
bool binNotChange(Plateau& p);
bool isInEchecAfterCardGreen(Plateau &p,std::function<bool(Plateau&, std::vector<gf::Vector2i> tabVector)> execute);

//card
bool NoCard (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool NoCardPlayable (Plateau& p, Phase f);

bool Princess (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool BombeAtomique (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Vampirisme (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool VisitesOfficielles (Plateau& p, std::vector<gf::Vector2i> tabVector); 

bool PrincessIsPlayable (Plateau& p, Phase f);
bool BombeAtomiqueIsPlayable (Plateau& p, Phase f);
bool VampirismeIsPlayable (Plateau& p, Phase f);
bool VisitesOfficiellesIsPlayable (Plateau& p, Phase f);

#endif
