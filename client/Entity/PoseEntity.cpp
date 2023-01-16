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
    
    gf::Vector2i sizeCard = gf::Vector2i(200+10,300+10);

    gf::Vector2f position = coords.getRelativePoint({ 0.2f, 0.4f });
    
    gf::RoundedRectangleShape contour(sizeCard);
    contour.setColor(gf::Color::fromRgba32(0.f, 0.f, 0.f, 0.f));
    contour.setOutlineColor(gf::Color::Yellow);
    contour.setOutlineThickness(6.f);
    contour.setRadius(22);
    contour.setPosition(position);
    contour.setAnchor(gf::Anchor::Center);

    gf::RectangleShape placementTexte;
    placementTexte.setSize({90.0f,1.0f});
    placementTexte.setColor(gf::Color::Black);
    placementTexte.setOutlineThickness(6.f);
    placementTexte.setOutlineColor(gf::Color::Black);
    placementTexte.setPosition({position.x,position.y-sizeCard.y/2.f});
    placementTexte.setAnchor(gf::Anchor::Center);

    gf::Text poseName("Carte jouée", m_poseNameFont, 16.f);
    poseName.setColor(gf::Color::White);
    poseName.setPosition({position.x,position.y-sizeCard.y/2.f});
    poseName.setAnchor(gf::Anchor::Center);

    target.draw(contour, states);

    target.draw(placementTexte,states);
    target.draw(poseName,states);

}
