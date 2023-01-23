#ifndef QUIT_SCENE_H
#define QUIT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Event.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>


struct GameHub;

class QuitScene : public gf::Scene {
	public:
		QuitScene(GameHub& game);

	private:
		void doProcessEvent(gf::Event& event) override;
		void doHandleActions(gf::Window& window) override;
		void doRender(gf::RenderTarget& target, const gf::RenderStates &states) override;

		void doShow() override;

		GameHub& m_game;

		gf::Action m_upAction;
		gf::Action m_downAction;
		gf::Action m_triggerAction;

		gf::WidgetContainer m_widgets;
		gf::TextButtonWidget m_resumeButton;
		gf::TextButtonWidget m_quitButton;

};



#endif // QUIT_SCENE_H