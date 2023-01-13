#include "RulesEntity.hpp"



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

    gf::Text gameRules("Regles du jeu", m_rulesFont, height/75+width/20);    
    gameRules.setColor(gf::Color::Yellow);
    gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.1f}));
    gameRules.setAnchor(gf::Anchor::TopCenter);
    gameRules.setOutlineColor(gf::Color::Black);
    gameRules.setOutlineThickness(titleCharacterSize/4.f);

    std::string rules[4];
    rules[0] = "Le jeu d'une carte ne peut pas conduire et ne conduit pas directement à une situation d'échec et mat ou de capture du roi, pas plus qu'il n'empêche un roi de s'échapper d'une situation d'échec. Cette règle prévaut sur toute autre. Si une carte jouée contredit cette règle, elle est nulle, mais toujours considérée comme jouée, et donc défaussée.";
    rules[1] = "Il est interdit d'utiliser plus d'une carte par coup. Un joueur peut donc utiliser une carte, et une seule, lors du déplacement de son adversaire.";
    rules[2] = "L'effet d'une carte peut compléter, remplacer ou modifier le coup joué. Certains effets ne sont que superposés à ce coup. Ceci est précisé sur chaque carte. Un coup peut être joué sans qu'aucune carte ne soit utilisée.";
    rules[3] = "Certaines cartes peuvent devenir inefficaces en raison de la disparition des pièces concernées.";

    gf::Text paragraph;
    paragraph.setColor(gf::Color::White);
    paragraph.setLineSpacing(1.5f);
    paragraph.setFont(m_rulesFont);
    paragraph.setCharacterSize((height/100+width/100)*2);
    paragraph.setParagraphWidth(width);
    paragraph.setAlignment(gf::Alignment::Justify);
    paragraph.setString(rules[m_index]);
    paragraph.setPosition(coords.getRelativePoint({ 0.5f, 0.45f}));
    paragraph.setAnchor(gf::Anchor::Center);

    target.draw(paper, states);
    target.draw(gameRules, states);
    target.draw(paragraph);
}