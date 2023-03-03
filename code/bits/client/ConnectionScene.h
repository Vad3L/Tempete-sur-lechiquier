#ifndef TSL_CONNECTION_SCENE_H
#define TSL_CONNECTION_SCENE_H

#include <gf/Scene.h>

#include "ConnectionEntity.h"
#include "../common/Network.h"
#include "GameModel.h"

namespace tsl {

    struct GameHub;	

    class ConnectionScene : public gf::Scene {
        public:
            ConnectionScene(GameHub& game, Network &network);
            
        private:
            void doProcessEvent(gf::Event& event) override;
            void doUpdate(gf::Time time) override;
            void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

            GameHub& m_game;
            Network& m_network;
            GameModel& m_model;
            
            std::array<char, 256> m_hostnameBuffer;
            bool m_connectionAsked;      

            ConnectionChoice m_choice;
            ConnectionEntity m_connection;
    };
}

#endif // TSL_CONNECTION_SCENE_H