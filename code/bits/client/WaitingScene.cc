#include "WaitingScene.h"
#include "GameHub.h"
#include "common/Protocol.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/Sprite.h>

namespace tsl {

    WaitingScene::WaitingScene(GameHub& game, Network& network)
    : gf::Scene(game.getRenderer().getSize())
    , m_game(game)
    , m_network(network)
    , m_model(m_game.m_model)
    , m_waiting(m_game.resources, m_game.m_model)
    {
        setClearColor(gf::Color::Black);

        addHudEntity(m_waiting);
    }

    void WaitingScene::doUpdate([[maybe_unused]] gf::Time time) {
        if (!isActive()) {
            return;
        }

        gf::Packet packet;

        while (m_network.queue.poll(packet)) {
            switch (packet.getType()) {
                case ColorDeck::type: {
                    gf::Log::debug("(GAME) receive Color Deck\n");
                    auto data = packet.as<ColorDeck>();
    
                    m_model.chessStatus = ChessStatus::NO_STARTED;
                    m_model.chessColor = data.colorPlayer;      
                    m_model.cards = data.hand;
                    break;
                }
                case StartGame::type: {
                    gf::Log::debug("(GAME) receive Start\n");

                    m_waiting.startCount();
                    break;
                }
            }
        }

        if(m_model.chessStatus == ChessStatus::ON_GOING) {
            m_game.replaceScene(*m_game.game, m_game.blackoutEffect, gf::seconds(0.4f));
        }
    }

}