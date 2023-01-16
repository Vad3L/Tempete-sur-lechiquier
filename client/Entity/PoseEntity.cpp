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

    gf::Coordinates coords({500,500});
    
    gf::Vector2i sizeCard = gf::Vector2i(200,300);
    gf::Vector2f position = coords.getRelativePoint({ -0.4f, -0.25f });
    
    gf::RoundedRectangleShape contour(sizeCard+10.f);
    contour.setColor(gf::Color::fromRgba32(0.f, 0.f, 0.f, 0.f));
    contour.setOutlineColor(gf::Color::White);
    contour.setOutlineThickness(10.f);
    contour.setRadius(22);
    contour.setPosition(position);

    target.draw(contour, states);
}
