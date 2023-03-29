#include "Card.h"

namespace tsl {
    
    Card::Card(int num, std::string name, std::map<std::string, std::string> description, Turn turn, Action action, Effect effect, std::size_t nbClickPossible) 
    : m_num(num)
    , m_name(name)
    , m_description(description)
    , m_turn(turn)
    , m_action(action)
    , m_effect(effect)
    , m_nbClickPossible(nbClickPossible)
    , m_execute(NoCard)
    , m_isPlayable(NoCardPlayable)
    {
        
    }

    Card::Card () 
    : m_num(-1)
    , m_name("")
    , m_turn(Turn::NONE)
    , m_action(Action::NONE)
    , m_effect(Effect::NONE)
    , m_nbClickPossible(0)
    , m_execute(NoCard)
    , m_isPlayable(NoCardPlayable)
    {
        
    }

}