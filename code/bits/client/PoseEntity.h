#ifndef TSL_POSE_ENTITY_H
#define TSL_POSE_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/RenderTarget.h>

#include "GameModel.h"
#include "CardEntity.h"

namespace tsl {

    class PoseEntity : public gf::Entity {
        public:
            PoseEntity(gf::ResourceManager& resources,GameModel &model);
            
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override ;

            void returnCardHand();
                    
            int m_cardPlaying;
            int m_cardDiscard;
            
        private:
            GameModel &m_model;

            gf::Font& m_poseNameFont;

            CardEntity m_cardEntity;
    };
}

#endif // TSL_POSE_ENTITY_H
