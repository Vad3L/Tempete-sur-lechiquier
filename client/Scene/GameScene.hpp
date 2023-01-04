#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/Font.h>
#include <gf/Text.h>

#include "../Network.hpp"
#include "../../model/Plateau.hpp"
#include "../../protocole/protocole.h"

#include "../Entity/BoardEntity.hpp"

struct GameHub;

class GameScene : public gf::Scene {
    public:
        GameScene(GameHub& game);
        gf::Vector2i transformInSelectedCase(gf::Vector2i mouseCoord);
        

    private:
        void doHandleActions(gf::Window& window) override;
        void doProcessEvent(gf::Event& event) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doUpdate(gf::Time time) override;

        void onActivityChange(bool active)  override;
    private:
        GameHub& m_game;

        gf::Action m_quitAction;
        gf::Action m_fullscreenAction;

        bool m_myTurn;
        ChessColor m_myColor;

        Plateau m_plateau;
        Network m_network;
        gf::Packet m_packet;

        BoardEntity m_boardEntity;    
        gf::View l_view;
};  



#endif // GAME_SCENE_H
