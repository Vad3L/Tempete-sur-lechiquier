#ifndef PLAY_SELECT_SCENE_H
#define PLAY_SELECT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/Coordinates.h>

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

#include "../Entity/PlayTitleEntity.hpp"

struct GameHub;

class PlaySelectScene : public gf::Scene {
    public:
        PlaySelectScene(GameHub& game);

    private:
        void doProcessEvent(gf::Event& event) override;
        void doHandleActions(gf::Window& window) override;
        void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
        void doShow() override;
        void changeRightLeft(bool value);
        void onActivityChange(bool active) override;
    private:
        GameHub& m_game;
        
        gf::Action m_quitAction;

        gf::Action m_leftAction;
        gf::Action m_rightAction;

        gf::Action m_triggerAction;
        gf::Action m_connectAction;
        gf::Action m_fullscreenAction;
        
        PlayTitleEntity m_PlayTitleEntity;

        gf::TextButtonWidget m_leftWidget;
        gf::TextButtonWidget m_rightWidget;

        gf::WidgetContainer m_widgets;
        gf::TextButtonWidget m_ipWidget;
        std::map<int, std::pair<std::string, std::string>> m_listIp; // IP et nameServer (adreese est unique)
        int m_index;
        
};



#endif // PLAY_SELECT_SCENE_H
