#ifndef SETTINGS_SCENE_H
#define SETTINGS_SCENE_H

#include "../Entity/RulesEntity.hpp"

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Sprite.h>

#include "../Entity/SettingsEntity.hpp"

struct GameHub;	
struct GameData;

class SettingsScene : public gf::Scene {
	public:
		SettingsScene(GameHub& game, GameData &gameData);

	private:
		void doProcessEvent(gf::Event& event) override;
		void doHandleActions(gf::Window& window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
		void doShow() override;
		void onActivityChange(bool active) override;
		
		void changeTexture(char c);
		void changeSound(char c);
	
		GameHub &m_game;
		GameData &m_gameData;

		gf::Action m_leftAction;
		gf::Action m_rightAction;
		gf::Action m_downAction;
		gf::Action m_upAction;
		gf::Action m_triggerAction;
		gf::Action m_fullscreenAction;
		gf::Action m_quitAction;

		gf::SpriteWidget m_quitButton;
		gf::SpriteWidget m_rightStyleButton;
		gf::SpriteWidget m_leftStyleButton;
		gf::SpriteWidget m_rightSoundButton;
		gf::SpriteWidget m_leftSoundButton;
	
		gf::WidgetContainer m_widgets;

		SettingsEntity m_settingsEntity;

		int m_ligne = 0;

};

#endif // SETTINGS_SCENE_H