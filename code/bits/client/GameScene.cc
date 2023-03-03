#include "GameScene.h"
#include "GameHub.h"

namespace tsl {

    GameScene::GameScene(GameHub& game, Network& network)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_model(m_game.m_model)
    , m_info(m_game.resources, m_model)
    {
        setClearColor(gf::Color::Violet);

        addHudEntity(m_info);
    }


}