#ifndef TSL_WAITING_ENTITY_H
#define TSL_WAITING_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Animation.h>
#include <gf/AnimatedSprite.h>

#include "GameModel.h"

namespace tsl {

    class WaitingEntity : public gf::Entity {
        public:
            WaitingEntity(gf::ResourceManager& resources, GameModel& model);

            void update(gf::Time time) override;
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

            void startCount();

        private:
            GameModel& m_model;

            float m_startTime;

			sf::Sound m_clock;

            gf::Texture& m_load;
            gf::Animation m_animation;
			gf::AnimatedSprite m_Animatedload;
            gf::Font& m_font;
    };

}

#endif // TSL_WAITING_ENTITY_H