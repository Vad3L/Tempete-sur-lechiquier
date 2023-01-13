#ifndef RULES_ENTITY_H
#define RULES_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Shapes.h>

#include <iostream>

class RulesEntity : public gf::Entity {
    public:
        RulesEntity(gf::ResourceManager& resources);
        void update(gf::Time time) override;
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        int m_index;

    private:
        gf::Font& m_font;
        gf::Texture& m_backgroundTexture;
        gf::Font& m_rulesFont;


};


#endif // HG_CREDIT_ENTITY_H