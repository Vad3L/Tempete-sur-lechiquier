#include "PoseEntity.hpp"

PoseEntity::PoseEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
, m_accessories(resources.getTexture("images/AccesoriesCards.png"))
, m_gameData(gameData)
{

}

void PoseEntity::update([[maybe_unused]] gf::Time time) {
}

void PoseEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

    gf::Coordinates coords(target);
    
    gf::Vector2i sizeCard = gf::Vector2i(200,300);
    gf::Vector2f position1 = coords.getRelativePoint({ 0.2f, 0.4f });
    gf::Vector2f position2 = coords.getRelativePoint({ 0.8f, 0.4f });

    gf::RoundedRectangleShape contour(sizeCard+10.f);
    contour.setColor(gf::Color::fromRgba32(0.f, 0.f, 0.f, 0.f));
    contour.setOutlineColor(gf::Color::Yellow);
    contour.setOutlineThickness(6.f);
    contour.setRadius(22);
    contour.setAnchor(gf::Anchor::Center);
    contour.setPosition(position1);

    target.draw(contour, states);

    contour.setPosition(position2);
    contour.setOutlineColor(gf::Color::Red);
    target.draw(contour, states);
}
