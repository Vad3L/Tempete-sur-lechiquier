#ifndef TSL_START_ENTITY_H
#define TSL_START_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Font.h>

namespace tsl {

    class StartEntity : public gf::Entity {
        public:
            StartEntity(gf::ResourceManager& resources);
        
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            gf::Font& m_font;
    };

}

#endif // TSL_START_ENTITY_H