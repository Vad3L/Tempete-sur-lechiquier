#ifndef CARD_H
#define CARD_H
#include <string>
#include <iostream>
#include <set>
#include <algorithm>
#include <functional>

#include "Plateau.hpp"
#include "CardFunction.hpp"

enum class Turn{
    BEFORE,
    AFTER,
    BOTH,
    DURING_TOUR_ADVERSE
};

enum class Action{
    CHOOSE_CASES, //plateau et/ou pioche
    NONE //en general ca depend d'un mouvement precendent par exemple bombe atomique)
};

enum class Effect{ // emum pour determiner dans quelle phase allons nous nous trouver une fois qu'on l'aura jouer
    REPLACE_COUP, // remplace ton coup normal 
    REGAME_COUP, // par exmeple en cas d'annultion 
    REGAME_OTHER_CARTE, // par exemple en cas d'annulation ou la carte qui permet de jouer une carte supplémentaire(rebelote dix de der)
    NONE // phase suivante(soit fin ou coup normal)
};

class Card{
    private:
        std::string m_name;
        std::string m_description;
        int m_num = 29;
        Turn m_turn;
        Effect m_effect;

    public:
	    std::function<void(Plateau&, gf::Vector2i, gf::Vector2i)> m_execute;
	    std::function<bool(Plateau&, Phase)> m_isPlayable;
	
        Card(std::string name,std::string m_description,Turn turn, Effect effect);
        Card();
        Turn getTurn(); //return when we can use the card b for beforeTurn, a for after
        Effect getEffect();
        std::string getName(); // return the name of the card
        int getNum();
};

#endif // CARD_H
