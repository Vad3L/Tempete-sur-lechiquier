#include "PoseEntity.hpp"

PoseEntity::PoseEntity(gf::ResourceManager& resources,GameData &gameData)
: m_cardsIllustration(resources.getTexture("images/CardIlustrationSheet.png"))
, m_accessories(resources.getTexture("images/AccesoriesCards.png"))
, m_gameData(gameData)
, m_poseNameFont(resources.getFont("fonts/DejaVuSans.ttf"))
{

}

void PoseEntity::update([[maybe_unused]] gf::Time time) {
}

void PoseEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

    gf::Coordinates coords(target);
    
    gf::Vector2i sizeCard = gf::Vector2i(200,300);

    gf::Vector2f position = coords.getRelativePoint({ -1.f, -0.25f });
    
    gf::RoundedRectangleShape contour(sizeCard+10.f);
    contour.setColor(gf::Color::fromRgba32(0.f, 0.f, 0.f, 0.f));
    contour.setOutlineColor(gf::Color::Yellow);
    contour.setOutlineThickness(6.f);
    contour.setRadius(22);
    contour.setAnchor(gf::Anchor::Center);
    contour.setPosition(position);

    gf::RectangleShape placementTexte;
    placementTexte.setSize({90.0f,1.0f});
    placementTexte.setColor(gf::Color::Black);
    placementTexte.setOutlineThickness(6.f);
    placementTexte.setOutlineColor(gf::Color::Black);
    placementTexte.setPosition({position.x+110.f,position.y-4.f});
    placementTexte.setAnchor(gf::Anchor::Center);

    gf::Text poseName("Carte joué", m_poseNameFont, 16.f);
    poseName.setColor(gf::Color::White);
    poseName.setPosition({position.x+110.f,position.y-3.f});
    poseName.setAnchor(gf::Anchor::Center);

    target.draw(contour, states);

    target.draw(placementTexte,states);
    target.draw(poseName,states);

}
