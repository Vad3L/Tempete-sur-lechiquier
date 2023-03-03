#include "StartEntity.h"
#include "../common/Constants.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>

namespace tsl {

    StartEntity::StartEntity(gf::ResourceManager& resources, GameModel& model)
    : m_model(model)
    ,m_font(resources.getFont("fonts/RifficFree-Bold.ttf"))
    
    {
        
    }

    void StartEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        gf::Text instructions(m_model.getWord("Press enter to start"), m_font);
        instructions.setScale(scale);
        instructions.setColor(gf::Color::White);
        instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.91f }));
        instructions.setAnchor(gf::Anchor::Center);
        
        const std::time_t result = std::time(nullptr);
        if((result % 2) ) {
            target.draw(instructions, states);
        }
    }

}