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
#include "../Entity/TableBoardEntity.hpp"
#include "../GameData.hpp"

struct GameHub;
struct GameData;

class GameScene : public gf::Scene {
    public:
        GameScene(GameHub& game);

    private:
        void doHandleActions(gf::Window& window) override;
        void doProcessEvent(gf::Event& event) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doUpdate(gf::Time time) override;

        void onActivityChange(bool active)  override;
    private:
        GameHub& m_game;
        GameData m_gameData;

        gf::Action m_quitAction;
        gf::Action m_fullscreenAction;

        
        Network m_network;
        gf::Packet m_packet;
    
        gf::LockedView m_boardView;

        BoardEntity m_boardEntity;    
        TableBoardEntity m_tableBoardEntity;
};  



#endif // GAME_SCENE_H
