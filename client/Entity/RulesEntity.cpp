#include "RulesEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include <iostream>

RulesEntity::RulesEntity(gf::ResourceManager& resources, int &index)
: m_font(resources.getFont("Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("startMenu1.png"))
, m_rulesFont(resources.getFont("RifficFree-Bold.ttf"))
, m_index(index)
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
    rules[0] = "Line breaking, also known as word wrapping, is the process of breaking a section of text into lines such that it will fit in the available width of a page, window or other display area.\nIn text display, line wrap is the feature of continuing on a new line when a line is full, such that each line fits in the viewable window, allowing text to be read from top to bottom without any horizontal scrolling.\nWord wrap is the additional feature of most text editors, word processors, and web browsers, of breaking lines between words rather than within words, when possible. Word wrap makes it unnecessary to hard-code newline delimiters within paragraphs, and allows the display of text to adapt flexibly and dynamically to displays of varying sizes.\nLine breaking, also known as word wrapping, is the process of breaking a section of text into lines such that it will fit in the available width of a page, window or other display area.\nIn text display, line wrap is the feature of continuing on a new line when a line is full, such that each line fits in the viewable window, allowing text to be read from top to bottom without any horizontal scrolling.\nWord wrap is the additional feature of most text editors, word processors, and web browsers, of breaking lines between words rather than within words, when possible. Word wrap makes it unnecessary to hard-code newline delimiters within paragraphs, and allows the display of text to adapt flexibly and dynamically to displays of varying sizes.";
    rules[1] = "America's a nation that can be defined in a single word: awdsmfafoothimaaafootafootwhscuseme.";
    rules[2] = "Seated prisoners, chained so that they cannot move their heads, stare at a cave wall on which are projected images. These images are cast from carved figures illuminated by a fire and carried by people on a parapet above and behind the prisoners. A prisoner is loosed from his chains. First he sees the carved images and the fire. Then he is led out of the cave into ‘the real’ world. Blinded by the light of the sun, he cannot look at the trees, rocks and animals around him, but instead looks at the shadows and reflections (in water) cast by those objects. As he becomes acclimatized, he turns his gaze (1) to those objects and finally, fully acclimatized, he looks to the source of illumination, the sun itself";
    rules[3] = "My heart aches, and a drowsy numbness pains My sense, as though of hemlock I had drunk, Or emptied some dull opiate to the drains One minute past, and Lethe-wards had sunk: 'Tis not through envy of thy happy lot, But being too happy in thine happiness,— That thou, light-winged Dryad of the trees In some melodious plot Of beechen green, and shadows numberless, Singest of summer in full-throated ease.";

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