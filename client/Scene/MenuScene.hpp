#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Event.h>
#include <gf/Text.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/Sprite.h>

#include <SFML/Audio.hpp>

struct GameHub;

class MenuScene : public gf::Scene {
	public:
		MenuScene(GameHub& game);

	private:
		void doProcessEvent(gf::Event& event) override;
		void doHandleActions(gf::Window& window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;
		void doShow() override;
		void onActivityChange(bool active) override;

		GameHub& m_game;

		gf::Action m_upAction;
		gf::Action m_downAction;
		gf::Action m_triggerAction;
		gf::Action m_quitAction;
		gf::Action m_fullscreenAction;
		
		gf::SpriteWidget m_play;
		gf::SpriteWidget m_rules;
		gf::SpriteWidget m_quit;
		gf::SpriteWidget m_settings;

		float m_cog;

		gf::Texture& m_backgroundTexture;

		gf::WidgetContainer m_widgets;

		sf::Sound btnClicked;
};


#endif // MENU_SCENE_H
