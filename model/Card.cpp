#include "Card.hpp"

Card::Card(std::string name,std::string description,Turn turn, Effect effect,int num){
    m_num = num;
    m_name = name;
    m_description = description;
    m_turn = turn;
    m_effect = effect;
    m_execute = NoCard;
    m_isPlayable = NoCardPlayable;
}

Card::Card () {
	gf::Log::info("jappelle\n");
    m_execute = NoCard;
	m_isPlayable = NoCardPlayable;
    m_num = -1;
}