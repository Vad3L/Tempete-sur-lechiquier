#include "ConnectionEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace tsl {

    ConnectionEntity::ConnectionEntity(gf::ResourceManager& resources, ConnectionChoice& choice, GameModel& model)
    : m_model(model)
    , m_connectionWidget(resources.getTexture("images/buttons/RightArrow.png"), resources.getTexture("images/buttons/RightArrow.png"), resources.getTexture("images/buttons/RightArrowSelected.png"))
    , m_quitWidget(resources.getTexture("images/buttons/Close.png"), resources.getTexture("images/buttons/Close.png"), resources.getTexture("images/buttons/CloseSelected.png"))
    , m_paper(resources.getTexture("images/Connection.png"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        m_paper.setSmooth(true);

        m_connectionWidget.setCallback([&choice]() { choice = ConnectionChoice::Connection; });
        m_container.addWidget(m_connectionWidget);

        m_quitWidget.setCallback([&choice]() { choice = ConnectionChoice::Quit; });
        m_container.addWidget(m_quitWidget);
    }
    
    void ConnectionEntity::pointTo(gf::Vector2f coords) {
        m_container.pointTo(coords);
    }

    void ConnectionEntity::triggerAction() {
        m_container.triggerAction();
    }

    void ConnectionEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        m_quitWidget.setScale(scale);
        m_quitWidget.setPosition(coords.getRelativePoint({ 0.65f, 0.15f }));
        m_quitWidget.setAnchor(gf::Anchor::Center);

        m_connectionWidget.setScale(scale * 0.5f);
        m_connectionWidget.setPosition(coords.getRelativePoint({ 0.59f, 0.505f }));
        m_connectionWidget.setAnchor(gf::Anchor::Center);

        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
        paper.setAnchor(gf::Anchor::Center);

        gf::Text connection(m_model.getWord("Connecting"), m_font, 50);
        connection.setScale(scale);
        connection.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        connection.setPosition(coords.getRelativePoint({ 0.5f, 0.26f }));
        connection.setAnchor(gf::Anchor::Center);

        gf::Text hostname(m_model.getWord("Hostname"), m_font);
        hostname.setScale(scale);
        hostname.setColor(gf::Color::fromRgba32(99, 63, 91, 255));
        hostname.setPosition(coords.getRelativePoint({ 0.5f, 0.44f }));
        hostname.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
        m_container.render(target, states);	
        target.draw(connection, states);
        target.draw(hostname, states);   
    }

}