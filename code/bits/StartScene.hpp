#ifndef HG_START_SCENE_HPP
#define HG_START_SCENE_HPP

#include <gf/Scene.h>
#include <gf/Action.h>

#include "TitleEntity.hpp"

namespace game {

  struct GameHub;

  class StartScene : public gf::Scene {
  public:
    StartScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;

    gf::Action m_startAction;
    gf::Action m_fullscreenAction;

    TitleEntity m_titleEntity;

  };

}

#endif // HG_START_SCENE_H
