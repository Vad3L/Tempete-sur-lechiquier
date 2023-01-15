#include "Card.hpp"

Card::Card () {
	m_execute = NoCard;
	m_isPlayable = NoCardPlayable;
}

Card::Card(std::string name,std::string description,Turn turn, Effect effect){
    m_name=name;
    m_description=description;
    m_turn=turn;
    m_effect=effect;
    m_execute = NoCard;
    m_isPlayable = NoCardPlayable;
}

Turn Card::getTurn() {
    return m_turn;
}

Effect Card::getEffect() {

    return m_effect;

}

std::string Card::getName() {
    return m_name;
}

int Card::getNum() {
    return m_num;
}
