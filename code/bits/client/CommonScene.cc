#include "CommonScene.h"

#include "GameHub.h"
#include "Singletons.h"
#include "../common/ImGuiConstants.h"

#include <gf/Coordinates.h>
#include <gf/Sprite.h>
#include <imgui.h>
#include <imgui_impl_gf.h>

namespace tsl {

	CommonScene::CommonScene(GameHub& game)
	: gf::Scene(game.getRenderer().getSize())
	, m_game(game)
	, m_model(m_game.m_model)
	, m_quitAction("Quit")
	, m_fullscreenAction("Fullscreen")
	, m_backgroundTexture(m_game.resources.getTexture("images/StartMenu.jpg"))
	{
		setClearColor(gf::Color::Black);

		m_backgroundTexture.setSmooth(true);
		
		m_quitAction.addScancodeKeyControl(gf::Scancode::Escape);
		addAction(m_quitAction);

		m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
		addAction(m_fullscreenAction);

	}

	void CommonScene::doHandleActions([[maybe_unused]] gf::Window& window) {    
		if (m_fullscreenAction.isActive()) {
			if(!m_game.connection->isActive()) {
				window.toggleFullscreen();
			}
		}

		if (m_quitAction.isActive()) {
			window.close();
		}
	}

	void CommonScene::doProcessEvent(gf::Event& event) {
        ImGui_ImplGF_ProcessEvent(event);
	}

	void CommonScene::doUpdate(gf::Time time) {
        ImGui_ImplGF_Update(time);
	}

	void CommonScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states) {
		gf::Coordinates coords(target);
		const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
		const auto position = coords.getRelativePoint({ 0.15f, 0.25f });
		
		if(!m_game.game->isActive()) {
			gf::Sprite background(m_backgroundTexture);
			if(!m_game.start->isActive()) { background.setColor(gf::Color::Opaque(0.20f)); }
			background.setScale(scale);
			target.draw(background, states);
		}
		
		if(m_game.settings->isActive() || m_game.connection->isActive()) {
			return;
		}
		
		// UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        
        const ImVec2 sizeWindow = ImVec2(ImGui::GetWindowSize().x * scale.x * 1.1f, ImGui::GetWindowSize().y * scale.y * 1.2f);
        ImGui::SetNextWindowSize(sizeWindow);

        if (ImGui::Begin("language", nullptr, DefaultWindowFlags|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground)) {
            ImGui::SetWindowFontScale(coords.getRelativeCharacterSize(0.02f) * 0.06f);
			
			if (ImGui::Button(strcat((char*)m_model.getWord("Language").c_str(), ": "))){
				ImGui::OpenPopup("my_select_popup");
			}
				
			ImGui::SameLine();
			ImGui::TextUnformatted(m_model.getWord(m_model.language).c_str());

			if (ImGui::BeginPopup("my_select_popup")) {
				for (int i = 0; i < static_cast<int>(languages.size()); i++) {
					if (ImGui::Selectable(m_model.getWord(languages[i]).c_str())) {
						m_model.language = languages[i];
						m_model.writeSettings();
					}
				}
				ImGui::EndPopup();
			}
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
	}
}