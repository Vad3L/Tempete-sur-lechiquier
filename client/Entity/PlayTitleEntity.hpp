#ifndef PLAY_TITLE_ENTITY_H
#define PLAY_TITLE_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Text.h>

class PlayTitleEntity : public gf::Entity {
    public:
        PlayTitleEntity(gf::ResourceManager& resources);
        void update(gf::Time time) override;
        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        gf::Text m_errorText;
    private:
        gf::Font& m_font;
        gf::Texture& m_backgroundTexture;
};




#endif // PLAY_TITLE_ENTITY_H
