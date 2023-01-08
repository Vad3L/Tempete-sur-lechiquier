#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/Font.h>
#include <gf/Text.h>
#include <gf/Sleep.h>
#include <gf/Color.h>

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
        void setIp(std::string ip);
    private:
        void doHandleActions(gf::Window& window) override;
        void doProcessEvent(gf::Event& event) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doUpdate(gf::Time time) override;

        void onActivityChange(bool active)  override;
    private:
        std::string m_ip;

        GameHub& m_game;
        GameData m_gameData;

        gf::Action m_quitAction;
        gf::Action m_fullscreenAction;

	    bool m_gameStart;
    
        gf::ExtendView m_boardView;
        gf::ExtendView m_tableBoardView;        
        gf::ViewContainer m_views;

        BoardEntity m_boardEntity;    
        TableBoardEntity m_tableBoardEntity;
};  



#endif // GAME_SCENE_H
