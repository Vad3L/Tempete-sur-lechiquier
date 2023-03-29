#ifndef TSL_CARD_FUNC_H
#define TSL_CARD_FUNC_H

#include "Board.h"
#include <sstream>
#include <functional>

namespace tsl {
        
    //tools
    bool binNotChange(Board& p);
    bool isInEchecAfterCardGreen(Board &p,std::function<bool(Board&, std::vector<gf::Vector2i> tabVector)> execute);

    //card
    bool NoCard ([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector);
    bool NoCardPlayable ([[maybe_unused]] Board& p,[[maybe_unused]] Phase f);

    bool Princess (Board& p, std::vector<gf::Vector2i> tabVector);
    bool BombeAtomique (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Vampirisme (Board& p, std::vector<gf::Vector2i> tabVector);
    bool VisitesOfficielles (Board& p, std::vector<gf::Vector2i> tabVector); 

    bool PrincessIsPlayable (Board& p, Phase f);
    bool BombeAtomiqueIsPlayable (Board& p, Phase f);
    bool VampirismeIsPlayable (Board& p, Phase f);
    bool VisitesOfficiellesIsPlayable (Board& p, Phase f);
}

#endif //TSL_CARD_FUNC_H