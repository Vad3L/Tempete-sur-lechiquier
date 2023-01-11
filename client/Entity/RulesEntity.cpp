#include "RulesEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include <iostream>

RulesEntity::RulesEntity(gf::ResourceManager& resources)
: m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("images/startMenu1.png"))
, m_rulesFont(resources.getFont("fonts/RifficFree-Bold.ttf"))
, m_index(0)
{

}

void RulesEntity::update([[maybe_unused]] gf::Time time) {
}

void RulesEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
    float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;
    unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.04f);
    unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.02f);

    gf::Sprite background(m_backgroundTexture);
    background.setColor(gf::Color::Opaque(0.20f));
    background.setPosition(coords.getCenter());
    background.setAnchor(gf::Anchor::Center);
    background.setScale(backgroundScale);
    target.draw(background, states);

    float height = target.getSize().height/1.5;
    float width = target.getSize().width/2;

    gf::RectangleShape paper({width, height});
    paper.setAnchor(gf::Anchor::Center);
    paper.setPosition(coords.getRelativePoint({0.5f, 0.4f}));
    paper.setColor(gf::Color::fromRgba32(100,74,75,150));

    gf::Text gameRules("Game rules", m_rulesFont, height/75+width/20);    
    gameRules.setColor(gf::Color::Yellow);
    gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.1f}));
    gameRules.setAnchor(gf::Anchor::TopCenter);
    gameRules.setOutlineColor(gf::Color::Black);
    gameRules.setOutlineThickness(titleCharacterSize/4.f);

    std::string rules[4];
    rules[0] = "The play of a card cannot and does not lead directly to a checkmate or king capture situation, nor does it prevent a king from escaping from a check situation. This rule prevails over any other. If a played card contradicts this rule, it is void, but still considered played, and therefore discarded.";
    rules[1] = "It is forbidden to use more than one card per move. A player can therefore use one card, and only one, during his opponent's move.";
    rules[2] = "The effect of a card can complete, replace, or modify the move played. Some effects are only superimposed on this move. This is specified on each card. A move can be played without any cards being used.";
    rules[3] = "Some cards may become ineffective due to the disappearance of the pieces involved.";

    gf::Text paragraph;
    paragraph.setColor(gf::Color::fromRgba32(200-m_index*40,100+m_index*30,200-m_index*40,255-30*m_index));
    paragraph.setLineSpacing(1.5f);
    paragraph.setFont(m_rulesFont);
    paragraph.setCharacterSize(height/100+width/100);
    paragraph.setParagraphWidth(width);
    paragraph.setAlignment(gf::Alignment::Justify);
    paragraph.setString(rules[m_index]);
    paragraph.setPosition(coords.getRelativePoint({ 0.5f, 0.45f}));
    paragraph.setAnchor(gf::Anchor::Center);

    target.draw(paper, states);
    target.draw(gameRules, states);
    target.draw(paragraph);
}