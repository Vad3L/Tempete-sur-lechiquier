#ifndef TSL_CONNECTION_ENTITY_H
#define TSL_CONNECTION_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

namespace tsl {

    class ConnectionEntity : public gf::Entity {
        public:
            ConnectionEntity(gf::ResourceManager& resources);
            
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            gf::Texture& m_paper;
            gf::Font& m_font;
                           
    };

}

#endif // TSL_CONNECTION_ENTITY_H