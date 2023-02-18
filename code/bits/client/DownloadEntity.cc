#include "DownloadEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>

#include <cmath>

namespace tsl {

    namespace {
        static constexpr gf::Vector2i DownloadScreenSize = {405, 76};
        static constexpr float barIncrement = 100 / 6.f;
    }

    DownloadEntity::DownloadEntity(gf::Texture& tempTexture)
    : m_tempTexture(tempTexture)
    , m_animationLoaded(false)
    , m_percentage(0) {
        m_tempTexture.setSmooth(true);
    }

    void DownloadEntity::loadAnimation(std::vector<gf::Ref<gf::Texture>> textures) {
        m_textures = textures;

        m_animationLoaded = true;
    }

    void DownloadEntity::changeFrame() {
        if (!m_animationLoaded) {
            return;
        }

        m_percentage += barIncrement;
    }

    void DownloadEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        gf::Coordinates coordinates(target);

        if (!m_animationLoaded) {
            gf::Sprite sprite(m_tempTexture);
            
            float scaleFactor = coordinates.getWindowSize().y / static_cast<float>(DownloadScreenSize.height) * 0.1f;
            sprite.scale(scaleFactor);
    
            sprite.setPosition(coordinates.getCenter());
            sprite.setAnchor(gf::Anchor::Center);
            target.draw(sprite, states);
        }
        else {
            std::size_t numSpreadSheat = ((m_percentage==100) ? 99 : m_percentage) / 30;
            float col = (static_cast<int>(ceilf(m_percentage)) % 10 == 0) ? 0.0f : 0.5f;
            int mod = (numSpreadSheat != m_textures.size()-1) ? 30 : 15;
            float lig = 1/3.f * (static_cast<int>(ceilf(m_percentage)) % mod / 10);
            
            gf::Sprite sprite(m_textures[numSpreadSheat], gf::RectF::fromPositionSize({ col, lig }, { 0.5f, 1/3.f }));
            
            float scaleFactor = coordinates.getWindowSize().y / static_cast<float>(DownloadScreenSize.height) * 0.1f;
            sprite.scale(scaleFactor);
    
            sprite.setPosition(coordinates.getCenter());
            sprite.setAnchor(gf::Anchor::Center);
            target.draw(sprite, states);   
        }
    }
}