#ifndef TSL_CARD_H
#define TSL_CARD_H

#include <string>
#include <map>
#include <algorithm>
#include <functional>

#include "CardFunction.h"
#include "CardFunctionChooseCases.h"

namespace tsl {

    enum class Turn {
        AVANT_COUP,
        APRES_COUP,
        BOTH,
        PDT_TOUR_ADVERSE,
        NONE
    };

    enum class Action{
        NONE, //en general ca depend d'un mouvement precendent par exemple bombe atomique)
        CHOOSE_CASES //plateau et/ou pioche
    };

    enum class Effect{ // emum pour determiner dans quelle phase allons nous nous trouver une fois qu'on l'aura jouer
        NONE, // phase suivante(soit fin ou coup normal)
        REPLACE_COUP, // remplace ton coup normal 
        REGAME_COUP, // par exmeple en cas d'annultion 
        REGAME_OTHER_CARD // par exemple en cas d'annulation ou la carte qui permet de jouer une carte supplémentaire(rebelote dix de der)
    };

    class Card{
        public:
            int m_num;
            std::string m_name;
            std::map<std::string, std::string> m_description;
            
            Turn m_turn;
            Action m_action;
            Effect m_effect;
            std::size_t m_nbClickPossible;
            
            std::function<bool(Board&, std::vector<gf::Vector2i>)> m_execute;
		    std::function<bool(Board&, Phase)> m_isPlayable;

            Card(int num, std::string name, std::map<std::string, std::string> m_description, Turn turn, Action action, Effect effect, std::size_t nbClickPossible);
            Card();

    };

}

#endif // TSL_CARD_H