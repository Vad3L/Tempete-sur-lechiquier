#ifndef TSL_CONNECTION_SCENE_H
#define TSL_CONNECTION_SCENE_H

#include <gf/Scene.h>

#include "../common/Network.h"

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
            
            std::array<char, 256> m_hostnameBuffer;
            std::array<char, 256> m_nameBuffer;
            bool m_connectionAsked;      
    };
}

#endif // TSL_CONNECTION_SCENE_H