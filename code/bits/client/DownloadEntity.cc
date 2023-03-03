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
        static constexpr float barIncrement = 100 / 8.f;
    }

    DownloadEntity::DownloadEntity(gf::ResourceManager& resources, GameModel& model)
    : m_percentage(0)
    , m_model(model)
    , m_textures(std::vector<gf::Ref<gf::Texture>>({
			resources.getTexture("animations/Download_screen_0.png"),
			resources.getTexture("animations/Download_screen_1.png"),
			resources.getTexture("animations/Download_screen_2.png"),
			resources.getTexture("animations/Download_screen_3.png"),
		}))
    , m_backgroundTexture(resources.getTexture("images/DownloadBackground.jpg"))
    , m_title(resources.getTexture("images/" + model.language + "Title.png"))
    , m_font(resources.getFont("fonts/RifficFree-Bold.ttf"))
    , m_points("")
    , m_interval(gf::seconds(0))
    {
        m_backgroundTexture.setSmooth(true);
        m_title.setSmooth(true);
    }

    void DownloadEntity::changeFrame() {
        m_percentage += barIncrement;
    }

    void DownloadEntity::update([[maybe_unused]] gf::Time time) {
        m_interval += time;
        
        if(m_interval.asSeconds() > 0.6f) {
            m_interval = gf::Time::Zero;
            
            if(m_points.size()==3) {
                m_points = "";
            }
            m_points += ".";
        }
    }
    
    void DownloadEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        gf::Sprite background(m_backgroundTexture);
        background.setScale(scale);
        
        gf::Sprite title(m_title);
        title.setScale(scale * 2.5f);
        title.setPosition(coords.getRelativePoint({ 0.5f, 0.14f }));
        title.setAnchor(gf::Anchor::Center);

        std::size_t numSpreadSheat = ((m_percentage==100) ? 99.f : m_percentage) / 30;
        float col = (static_cast<int>(ceilf(m_percentage)) % 10 == 0) ? 0.0f : 0.5f;
        int mod = (numSpreadSheat != m_textures.size()-1) ? 30 : 15;
        float lig = 1/3.f * (static_cast<int>(ceilf(m_percentage)) % mod / 10);
        
        gf::Sprite sprite(m_textures[numSpreadSheat], gf::RectF::fromPositionSize({ col, lig }, { 0.5f, 1/3.f }));
        float scaleFactor = coords.getWindowSize().y / static_cast<float>(DownloadScreenSize.height) * 0.1f;
        sprite.scale(scaleFactor);
        sprite.setPosition(coords.getRelativePoint({ 0.5f, 0.86f }));
        sprite.setAnchor(gf::Anchor::Center);

        gf::Text download(m_model.getWord("Download"), m_font);
        download.setScale(scale);
        download.setColor(gf::Color::White);
        const gf::Vector2f pos = { (coords.getWindowSize().x - download.getLocalBounds().getWidth() * scale.x) * 0.5f, coords.getRelativePoint({0 , 0.92f }).y };
        download.setPosition(pos);
        download.setString( download.getString() + " " + m_points);
        download.setAnchor(gf::Anchor::CenterLeft);

        target.draw(background, states);
        target.draw(title, states);
        target.draw(sprite, states);
        target.draw(download, states);
    }
    
}