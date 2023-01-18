#include "Card.hpp"

Card::Card(int num, std::string name,std::string description,Turn turn, Action action, Effect effect){
	m_num = num;
	m_name = name;
	m_description = description;
	m_turn = turn;
	m_action = action;
	m_effect = effect;
	m_execute = NoCard;
	m_isPlayable = NoCardPlayable;
}

Card::Card () {
	m_execute = NoCard;
	m_isPlayable = NoCardPlayable;
	m_num = -1;
}