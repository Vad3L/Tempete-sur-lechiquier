#include "WaitingEntity.h"

#include <gf/Coordinates.h>
#include <gf/Sprite.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>

namespace tsl {

    WaitingEntity::WaitingEntity(gf::ResourceManager& resources, GameModel& model)
    : m_model(model)
    , m_startTime(10)
    , m_load(resources.getTexture("animations/Loading.png"))
    , m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
    {
        m_clock.setBuffer(gAudioManager().getSound("sounds/Clock.ogg"));
	    m_clock.setVolume(FxsVolume);
        
        m_load.setSmooth(true);

        m_animation.addTileset(m_load, gf::vec(12, 1), gf::milliseconds(400), 12);
	    m_Animatedload.setAnimation(m_animation);
  	    m_Animatedload.setOrigin({ 84.f / 2.0f, 84.f / 2.0f });
	    m_Animatedload.setAnchor(gf::Anchor::Center);
    }
    
    void WaitingEntity::update([[maybe_unused]] gf::Time time) {
        if(m_startTime <= 3.f) {
            m_startTime -= time.asSeconds();
        }

        if(m_startTime < 0.f) {
            m_model.chessStatus = ChessStatus::ON_GOING;
            m_clock.stop();
            m_startTime = 10.f;
        }

        m_Animatedload.update(time);
    }

    void WaitingEntity::startCount() {
        m_startTime = 3;
        gBackgroundMusic.stop();
        m_clock.setVolume(FxsVolume);
        m_clock.play();
    }

    void WaitingEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        if(gBackgroundMusic.getStatus() == sf::SoundSource::Stopped) {
            std::string indication = m_model.getWord("Game start in") + " " + std::to_string(static_cast<int>(m_startTime)) + 
                        " " + m_model.getWord("seconds ... You play") + " " + ((m_model.chessColor==ChessColor::WHITE) ? m_model.getWord("White") : m_model.getWord("Black"));
            indication[indication.find(".")+3] = '\n';
           
            gf::Text text(indication, m_font, 50);
            text.setColor(gf::Color::White);
            text.setScale(scale);
            text.setAlignment(gf::Alignment::Center);
            text.setParagraphWidth(1600.f);
            text.setLineSpacing(2.f);
            text.setPosition(coords.getRelativePoint({ 0.5f, 0.45f }));	
            text.setAnchor(gf::Anchor::Center);

            target.draw(text, states);
		}else {
            gf::Text text(m_model.getWord("Waiting player"), m_font, 50);
            text.setColor(gf::Color::White);
            text.setScale(scale);
            text.setPosition(coords.getRelativePoint({ 0.5f, 0.45f }));	
            text.setAnchor(gf::Anchor::Center);
              
            m_Animatedload.setPosition(coords.getRelativePoint({0.5f, 0.55f }));

            target.draw(text, states);
            target.draw(m_Animatedload, states);
        }

    }

}