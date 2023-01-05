#include "RulesEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>


RulesEntity::RulesEntity(gf::ResourceManager& resources)
: m_font(resources.getFont("Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("startMenu.png"))
, m_rulesFont(resources.getFont("Signature.otf"))
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
    unsigned gitCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text title("Rules", m_font, titleCharacterSize);
    title.setColor(gf::Color::White);
    title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
    title.setAnchor(gf::Anchor::Center);
    target.draw(title, states);

    gf::Text git("", m_rulesFont, gitCharacterSize);
    git.setParagraphWidth(20.0f);
    git.setString("Github: https://github.com/Vad3L/Tempete-sur-lechiquier");
    git.setColor(gf::Color::White);
    git.setPosition(coords.getRelativePoint({ 0.5f, 0.7f}));
    git.setAnchor(gf::Anchor::Center);

    gf::Text developper("", m_rulesFont, subtitleCharacterSize);
    developper.setParagraphWidth(10.0f);
    developper.setAlignment(gf::Alignment::Justify);
    developper.setString("Developper:"
                            " Anthony "
                            " Leo "
                            " Louis "
                            " Quentin ");
    developper.setColor(gf::Color::White);
    developper.setPosition(coords.getRelativePoint({ 0.1f, 0.4f}));
    developper.setAnchor(gf::Anchor::Center);
    
    target.draw(developper, states);
    target.draw(git, states);
}