#ifndef CARDS_ENTITY_H
#define CARDS_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>

#include "../GameData.hpp"


class CardsEntity : public gf::Entity {
    public:
    CardsEntity(gf::ResourceManager& resources,GameData &gameData);
    
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override ;

    private:
    gf::Font& m_font;
    gf::Font& m_CardsFont;


};


#endif // CARDS_ENTITY_H