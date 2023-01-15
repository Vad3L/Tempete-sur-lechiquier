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
    REPLACE,
    BOTH,
    DURING
};

enum class Effect{
    CANCELCARD,
    CANCELMOVE,
    CHOOSECASE,
    NONE
};

class Card{
    private:
        std::string m_name;
        std::string m_description;
        Turn m_turn;
        Effect m_effect;
        float m_id;
    public:
	    std::function<void(Plateau&, gf::Vector2i, gf::Vector2i)> m_execute;
	    std::function<bool(Plateau&, Phase)> m_isPlayable;
	
        Card(std::string name,std::string m_description,Turn turn, Effect effect,float id);
        Card();

        Turn getTurn() const; //return when we can use the card b for beforeTurn, a for after
        Effect getEffect() const;//return return r for replace,cm for CancelMove and cc for CancelCard
        std::string getName() const; // return the name of the card
        float getId() const;
};

#endif // CARD_H
