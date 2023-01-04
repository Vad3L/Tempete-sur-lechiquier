#ifndef PLAY_SELECT_SCENE_H
#define PLAY_SELECT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>

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

    private:
        GameHub& m_game;
        gf::Action m_quitAction;

        gf::Action m_upAction;
        gf::Action m_downAction;
        gf::Action m_triggerAction;

        PlayTitleEntity m_PlayTitleEntity;

        gf::WidgetContainer m_widgets;
        gf::TextButtonWidget m_join;

        
};



#endif // PLAY_SELECT_SCENE_H
