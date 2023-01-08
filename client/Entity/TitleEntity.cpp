#include "TitleEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <iostream>

TitleEntity::TitleEntity(gf::ResourceManager& resources)
: m_font(resources.getFont("DejaVuSans.ttf"))
, m_backgroundTexture(resources.getTexture("startMenu1.png"))
{
m_backgroundTexture.setSmooth(true);
}

void TitleEntity::update([[maybe_unused]] gf::Time time) {
}

void TitleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
    float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

    gf::Sprite background(m_backgroundTexture);
    background.setPosition(coords.getCenter());
    background.setAnchor(gf::Anchor::Center);
    background.setScale(backgroundScale);
    target.draw(background, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.03f);

    gf::Text instructions("Press a button to start", m_font, instructionsCharacterSize);
    instructions.setColor(gf::Color::White);
    instructions.setOutlineColor(gf::Color::Black);
    instructions.setOutlineThickness(instructionsCharacterSize * 0.05f);
    instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.91f }));
    instructions.setAnchor(gf::Anchor::Center);
    
    std::time_t result = std::time(nullptr);
    if((result % 2) ) {
        target.draw(instructions, states);
    }

    
}


