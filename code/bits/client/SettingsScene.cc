#include "SettingsScene.h"

#include "GameHub.h"
#include "../common/Constants.h"
#include "../common/ImGuiConstants.h"
#include  "Tools.h"

#include <gf/Coordinates.h>
#include <imgui.h>
#include <imgui_impl_gf.h>

namespace tsl {
    
    namespace {
        constexpr std::size_t nb_themes = 3;
        std::array<std::string, nb_themes> themes = { "Paper", "Marble", "Wood" };
    }

    SettingsScene::SettingsScene(GameHub& game)
    : gf::Scene({game.getRenderer().getSize()})
    , m_game(game)
    , m_model(m_game.m_model)
    , m_choice(SettingsChoice::None)
    , m_settings(m_game.resources, m_choice, m_game.m_model)
    {
        setClearColor(gf::Color::Black);

        addHudEntity(m_settings);
    }

    void SettingsScene::doProcessEvent(gf::Event& event) {
        ImGui_ImplGF_ProcessEvent(event);

        switch (event.type) {
            case gf::EventType::MouseMoved:
                m_settings.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
                break;

            case gf::EventType::MouseButtonPressed:
                m_settings.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                m_settings.triggerAction();
                break;

            default:
                break;
        }
    }

    void SettingsScene::doUpdate(gf::Time time) {
        if (!isActive()) {
			return;
		}
        
        ImGui_ImplGF_Update(time);
        
        FxsVolume = m_model.sound;
        BackgroundAmbiantVolume = (FxsVolume == 0.f) ? 0.f : m_model.music;
        gBackgroundMusic.setVolume(BackgroundAmbiantVolume);

        switch (m_choice) {
            case SettingsChoice::None:
                return;
            case SettingsChoice::Quit:
                m_model.writeSettings();
                m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
        }

        playClickButton();
        m_choice = SettingsChoice::None;
    }

    void SettingsScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        const auto position = coords.getRelativePoint({ 0.5f, 0.6f });
        
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
		style.Colors[ImGuiCol_FrameBg]               = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        style.GrabRounding = 15.0f;
        
        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        
        const ImVec2 sizeWindow = ImVec2(ImGui::GetWindowSize().x * scale.x * 1.1f, ImGui::GetWindowSize().y * scale.y * 1.2f);
        style.GrabMinSize = sizeWindow.x * 0.05f;

        ImGui::SetNextWindowSize(sizeWindow);

        if (ImGui::Begin("Settings", nullptr, DefaultWindowFlags|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground)) {
            ImGui::PushItemWidth(sizeWindow.x * 0.874f);
            ImGui::SetWindowFontScale(coords.getRelativeCharacterSize(0.02f) * 0.059f);

            style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.06f);
            ImGui::SliderInt("", &m_model.theme, 1, nb_themes);

            style.Colors[ImGuiCol_Text]                  = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
            ImGui::SetCursorPosX((sizeWindow.x - ImGui::CalcTextSize(m_model.getWord(themes[m_model.theme-1]).c_str()).x) * 0.5f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.06f);
            ImGui::Text("%s", m_model.getWord(themes[m_model.theme-1]).c_str());

            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.31f);
            ImGui::SliderInt(" ", &m_model.music, 0, 100);
            
            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.56f);
            ImGui::SliderInt("  ", &m_model.sound, 0, 100);    
        }

        ImGui::End();

        // Display  
        renderHudEntities(target, states);

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
        
    }

}