#include "DownloadEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/Log.h>

#include <cmath>

namespace tsl {

    namespace {
        static constexpr gf::Vector2i DownloadScreenSize = {405, 76};
        static constexpr float barIncrement = 100 / 6.f;
    }

    DownloadEntity::DownloadEntity(gf::ResourceManager& resources)
    : m_percentage(0)
    , m_textures(std::vector<gf::Ref<gf::Texture>>({
			resources.getTexture("animations/download_screen_0.png"),
			resources.getTexture("animations/download_screen_1.png"),
			resources.getTexture("animations/download_screen_2.png"),
			resources.getTexture("animations/download_screen_3.png"),
		}))
    , m_backgroundTexture(resources.getTexture("images/downloadBackground.jpg"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    , m_indication("Chargement ")
    , m_interval(gf::seconds(0))
    {
        
    }

    void DownloadEntity::changeFrame() {
        m_percentage += barIncrement;
    }

    void DownloadEntity::update([[maybe_unused]] gf::Time time) {
        m_interval += time;
        
        if(m_interval.asSeconds() > 0.6f) {
            m_interval = gf::Time::Zero;
            
            if(m_indication.size()==14) {
                m_indication = m_indication.substr(0, 11);
            }
            m_indication += ".";
        }
    }
    
    void DownloadEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        gf::Coordinates coordinates(target);

        const gf::Vector2f scale = coordinates.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        gf::Sprite background(m_backgroundTexture);
        background.setScale(scale);
        
        std::size_t numSpreadSheat = ((m_percentage==100) ? 99.f : m_percentage) / 30;
        float col = (static_cast<int>(ceilf(m_percentage)) % 10 == 0) ? 0.0f : 0.5f;
        int mod = (numSpreadSheat != m_textures.size()-1) ? 30 : 15;
        float lig = 1/3.f * (static_cast<int>(ceilf(m_percentage)) % mod / 10);
        
        gf::Sprite sprite(m_textures[numSpreadSheat], gf::RectF::fromPositionSize({ col, lig }, { 0.5f, 1/3.f }));
        
        float scaleFactor = coordinates.getWindowSize().y / static_cast<float>(DownloadScreenSize.height) * 0.1f;
        sprite.scale(scaleFactor);

        sprite.setPosition(coordinates.getRelativePoint({ 0.5f, 0.8f}));
        sprite.setAnchor(gf::Anchor::Center);


        gf::Text download(m_indication, m_font);
        download.setScale(scale);
        download.setColor(gf::Color::White);
        download.setPosition(coordinates.getRelativePoint({ 0.435f, 0.87f }));
        download.setAnchor(gf::Anchor::CenterLeft);

        target.draw(background, states);
        target.draw(sprite, states);
        target.draw(download, states);
    }
}