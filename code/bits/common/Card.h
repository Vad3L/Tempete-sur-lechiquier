#ifndef TSL_CARD_H
#define TSL_CARD_H

#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <functional>

namespace tsl {

    enum class Turn {
        AVANT_COUP,
        APRES_COUP,
        BOTH,
        DURING_TOUR_ADVERSE,
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
        REGAME_OTHER_CARTE // par exemple en cas d'annulation ou la carte qui permet de jouer une carte supplémentaire(rebelote dix de der)
    };

    class Card{
        public:
            std::string m_name;
            std::string m_description;
            
            Turn m_turn;
            Action m_action;
            Effect m_effect;
            std::size_t m_nbClickPossible;
            
            int m_num;

            Card(int num, std::string name, std::string m_description, Turn turn, Action action, Effect effect, std::size_t nbClickPossible);
            Card();

    };

}

#endif // TSL_CARD_H