#include "SettingsEntity.hpp"

#include "../GameData.hpp"

#include "../Singletons.hpp"

SettingsEntity::SettingsEntity(gf::ResourceManager& resources, GameData &gameData)
: m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_backgroundTexture(resources.getTexture("images/StartMenu.png"))
, m_gameData(gameData)
{
	m_ligne = 0;
}

void SettingsEntity::update([[maybe_unused]] gf::Time time) {
}

void SettingsEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
	gf::Coordinates coords(target);

	float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;

	gf::Sprite background(m_backgroundTexture);
	background.setColor(gf::Color::Opaque(0.20f));
	background.setPosition(coords.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);

	unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.1f);

	gf::Text title("Parametres", m_font, titleCharacterSize);
	title.setColor(gf::Color::White);
	title.setPosition(coords.getRelativePoint({ 0.5f, 0.1f }));
	title.setAnchor(gf::Anchor::TopCenter);

	unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.05f);
	
	gf::Text subtitleStyle("Style du plateau", m_font, subtitleCharacterSize);
	(m_ligne==0) ? subtitleStyle.setColor(gf::Color::fromRgba32(144, 129, 60)) : subtitleStyle.setColor(gf::Color::White);
	subtitleStyle.setPosition(coords.getRelativePoint({ 0.5f, 0.3f }));
	subtitleStyle.setAnchor(gf::Anchor::Center);

	gf::Text textureStyle("None", m_font, subtitleCharacterSize);
	textureStyle.setColor(gf::Color::Blue);
	textureStyle.setPosition(coords.getRelativePoint({ 0.5f, 0.4f }));

	gf::Text subtitleSound("Son du jeu", m_font, subtitleCharacterSize);
	(m_ligne==1) ? subtitleSound.setColor(gf::Color::fromRgba32(144, 129, 60)) : subtitleSound.setColor(gf::Color::White);
	subtitleSound.setPosition(coords.getRelativePoint({ 0.5f, 0.55f }));
	subtitleSound.setAnchor(gf::Anchor::Center);
	switch (m_gameData.m_style) {
		case 0:
			textureStyle.setString("Bois");
			break;
		case 1:
			textureStyle.setString("Marbre");
			break;
		case 2:
			textureStyle.setString("Papier");
			break;
	}
	textureStyle.setAnchor(gf::Anchor::Center);

	gf::Text sound(std::to_string((int)BackgroundAmbiantVolume), m_font, subtitleCharacterSize);
	sound.setColor(gf::Color::Blue);
	sound.setPosition(coords.getRelativePoint({ 0.5f, 0.65f }));
	sound.setAnchor(gf::Anchor::Center);
	
	target.draw(background, states);
	target.draw(title, states);
	target.draw(textureStyle, states);
	target.draw(subtitleStyle, states);
	target.draw(subtitleSound, states);
	target.draw(sound, states);
}

