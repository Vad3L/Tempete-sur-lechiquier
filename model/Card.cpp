#include "Card.hpp"

Card::Card () {
	m_execute = NoCard;
	m_isPlayable = NoCardPlayable;
    m_num = -1;
}

Card::Card(std::string name,std::string description,Turn turn, Effect effect,int num){
    m_num = num;
    m_name = name;
    m_description = description;
    m_turn = turn;
    m_effect = effect;
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

