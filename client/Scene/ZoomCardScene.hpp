#ifndef ZOOM_CARD_SCENE_H
#define ZOOM_CARD_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Event.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>

#include "../Entity/CardEntity.hpp"

struct GameHub;

class ZoomCardScene : public gf::Scene {
	public:
		ZoomCardScene(GameHub& game);

		Card m_cardZoom;
		
	private:
		void doProcessEvent(gf::Event& event) override;
		void doHandleActions(gf::Window& window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

		GameHub& m_game;

		gf::Action m_quitAction;
		gf::Action m_fullscreenAction;

		CardEntity m_cardEntity;
};



#endif // QUIT_SCENE_H