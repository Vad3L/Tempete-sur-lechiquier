#include "Card.hpp"

Card::Card(std::string name,std::string description,Turn turn, Effect effect){

    m_name=name;
    m_description=description;
    m_turn=turn;
    m_effect=effect;

}

Card::Card() {
    
}

Turn Card::getTurn() const{
    return m_turn;
}

Effect Card::getEffect() const{

    return m_effect;

}

std::string Card::getName() const{
    return m_name;
}

