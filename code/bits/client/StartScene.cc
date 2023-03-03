#include "StartScene.h"

#include "GameHub.h"

namespace tsl {

    StartScene::StartScene(GameHub& game)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_startAction("Start")
    , m_startEntity(m_game.resources, m_game.m_model)
    {
        setClearColor(gf::Color::Black);

        //m_startAction.addMouseButtonControl(gf::MouseButton::Left);
        m_startAction.addKeycodeKeyControl(gf::Keycode::Return);
        addAction(m_startAction);
        
        addHudEntity(m_startEntity);
    }

    void StartScene::doHandleActions([[maybe_unused]] gf::Window& window) {
        if (!isActive()) {
            return;
        }

        if (m_startAction.isActive()) {
            m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
        }
    }
}