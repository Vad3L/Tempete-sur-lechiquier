#ifndef SETTINGS_ENTITY_H
#define SETTINGS_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Text.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

struct GameData;

class SettingsEntity : public gf::Entity {
	public:
		SettingsEntity(gf::ResourceManager& resources, GameData &gameData);
		void update(gf::Time time) override;
		void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
		
	private:
		gf::Font& m_font;
		gf::Texture& m_backgroundTexture;

		GameData &m_gameData;
};




#endif // SETTINGS_ENTITY_H
