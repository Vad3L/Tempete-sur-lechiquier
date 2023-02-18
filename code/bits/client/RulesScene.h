#ifndef TSL_RULES_SCENE_H
#define TSL_RULES_SCENE_H

#include <gf/Scene.h>

#include "RulesEntity.h"

namespace tsl {

    struct GameHub;	

    class RulesScene : public gf::Scene {
        public:
            RulesScene(GameHub& game);
    
        private:
            void doProcessEvent(gf::Event& event) override;
            void doUpdate(gf::Time time) override;

            GameHub& m_game;
            
            RulesChoice m_choice;
            RulesEntity m_rules;
    };

}

#endif // TSL_RULES_SCENE_H