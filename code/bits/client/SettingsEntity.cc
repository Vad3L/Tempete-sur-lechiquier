#include "SettingsEntity.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace tsl {

    namespace {
        constexpr std::size_t nb_page = 4;
    }

    SettingsEntity::SettingsEntity(gf::ResourceManager& resources)
    : m_paper(resources.getTexture("images/settings.png"))
    {
        m_paper.setSmooth(true);

    }
    
    void SettingsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);

        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        
        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.5f }));
        paper.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
    }

}