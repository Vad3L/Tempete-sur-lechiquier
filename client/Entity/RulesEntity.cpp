#include "RulesEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>


RulesEntity::RulesEntity(gf::ResourceManager& resources)
: m_font(resources.getFont("Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("startMenu1.png"))
, m_rulesFont(resources.getFont("RifficFree-Bold.ttf"))
{

}

void RulesEntity::update([[maybe_unused]] gf::Time time) {
}

void RulesEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
    float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

    gf::Sprite background(m_backgroundTexture);
    background.setColor(gf::Color::Opaque(0.20f));
    background.setPosition(coords.getCenter());
    background.setAnchor(gf::Anchor::Center);
    background.setScale(backgroundScale);
    target.draw(background, states);

    unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);
    unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.07f);

    /*gf::Text title("Storm on Chess", m_font, titleCharacterSize);
    title.setCharacterSize(50);
    title.setAlignment(gf::Alignment::Center);
    title.setColor(gf::Color::Yellow);
    title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
    title.setAnchor(gf::Anchor::Center);
    title.setOutlineColor(gf::Color::Black);
    title.setLineSpacing(0.7f);
    title.setRotation(gf::Pi/-10.f);
    title.setOutlineThickness(2.5f);

    target.draw(title, states);*/

    gf::RectangleShape tableCloth(coords.getRelativePoint({0.8f, 0.7f}));
    tableCloth.setAnchor(gf::Anchor::Center);
    tableCloth.setPosition(coords.getRelativePoint({0.5f, 0.4f}));
    tableCloth.setColor(gf::Color::fromRgba32(100,74,75,150));
    tableCloth.setScale(backgroundScale);

    gf::Text gameRules("Règles du jeux", m_rulesFont, subtitleCharacterSize);    
    gameRules.setColor(gf::Color::Yellow);
    gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.1f}));
    gameRules.setAnchor(gf::Anchor::TopCenter);
    gameRules.setOutlineColor(gf::Color::Black);
    gameRules.setOutlineThickness(5.f);


    target.draw(tableCloth, states);
    target.draw(gameRules, states);
}