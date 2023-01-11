#include "CardsEntity.hpp"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include <iostream>


CardsEntity::CardsEntity(gf::ResourceManager& resources,GameData &gameData)
: m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_CardsFont(resources.getFont("fonts/RifficFree-Bold.ttf"))
{

}

void CardsEntity::update([[maybe_unused]] gf::Time time) {
}



void CardsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states){

    gf::Coordinates coordsCard(gf::Vector2f(0.5f , 0.5f));
    gf::RectangleShape card1({ 60.f , 100.f});
    card1.setColor(gf::Color::White);
    card1.setOutlineColor(gf::Color::Black);
    card1.setPosition(coordsCard.getRelativePoint({0.9f,0.9f}));
    target.draw(card1,states);

    /*float height = card1.getSize().height/1.5;
    float width = card1.getSize().width/2;*/
    gf::Coordinates coords(target);
    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.01f);

    gf::Text cardName("Camel", m_font, instructionsCharacterSize);
    cardName.setColor(gf::Color::White);
    cardName.setOutlineColor(gf::Color::White);
    cardName.setPosition(coordsCard.getRelativePoint({ 0.5f, 0.1f }));
    /*cardName.setAnchor(gf::Anchor::Center);*/
    target.draw(cardName,states);




}