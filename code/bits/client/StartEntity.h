#ifndef TSL_START_ENTITY_H
#define TSL_START_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Font.h>

#include "GameModel.h"

namespace tsl {

    class StartEntity : public gf::Entity {
        public:
            StartEntity(gf::ResourceManager& resources, GameModel& model);
        
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            GameModel& m_model;

            gf::Font& m_font;
    };

}

#endif // TSL_START_ENTITY_H