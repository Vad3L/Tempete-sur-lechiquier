#ifndef RULES_ENTITY_H
#define RULES_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>


class RulesEntity : public gf::Entity {
    public:
    RulesEntity(gf::ResourceManager& resources, int &index);
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
    
    private:
    gf::Font& m_font;
    gf::Texture& m_backgroundTexture;
    gf::Font& m_rulesFont;

    int &m_index;
};


#endif // HG_CREDIT_ENTITY_H