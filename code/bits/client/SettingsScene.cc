#include "SettingsScene.h"

#include "GameHub.h"
#include "../common/ImGuiConstants.h"

#include <gf/Coordinates.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

namespace tsl {
    
    namespace {
        constexpr std::size_t nb_themes = 3;
        std::array<std::string, nb_themes> themes = { "Bois", "Marbre", "Papier" };
    }

    SettingsScene::SettingsScene(GameHub& game)
    : gf::Scene({game.getRenderer().getSize()})
    , m_game(game)
    , m_settings(game.resources)
    , m_theme(1)
    , m_music(0)
    , m_sound(0)
    {
        setClearColor(gf::Color::Black);

        addHudEntity(m_settings);
    }

    void SettingsScene::doProcessEvent(gf::Event& event) {
        ImGui_ImplGF_ProcessEvent(event);
    }

    void SettingsScene::doUpdate(gf::Time time) {
        ImGui_ImplGF_Update(time);
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
		//style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		//style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        style.Colors[ImGuiCol_Button]                = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonActive]          = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
        style.GrabRounding = 15.0f;
        
        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        
        const ImVec2 sizeWindow = ImVec2(ImGui::GetWindowSize().x * scale.x * 1.1f, ImGui::GetWindowSize().y * scale.y * 1.2f);
        style.GrabMinSize = sizeWindow.x * 0.05f;

        ImGui::SetNextWindowSize(sizeWindow);

        if (ImGui::Begin("Settings", nullptr, DefaultWindowFlags|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground)) {
            ImGui::PushItemWidth(m_settings.m_paper.getSize().x * (scale.x / 1.56f));
            ImGui::SetWindowFontScale(coords.getRelativeCharacterSize(0.02f) * 0.059f);

            style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.06f);
            ImGui::SliderInt("", &m_theme, 1, nb_themes);

            style.Colors[ImGuiCol_Text]                  = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
            ImGui::SetCursorPosX((sizeWindow.x - ImGui::CalcTextSize(themes[m_theme-1].c_str()).x) * 0.5f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.06f);
            ImGui::Text("%s", themes[m_theme-1].c_str());

            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.31f);
            ImGui::SliderInt(" ", &m_music, 0, 100);
            
            ImGui::SetCursorPosX(sizeWindow.x * 0.065f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.56f);
            ImGui::SliderInt("  ", &m_sound, 0, 100);
            
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeWindow.x * 0.2f) * 0.5f);
            ImGui::SetCursorPosY(sizeWindow.y * 0.74f);
            
            style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
            
            if (ImGui::Button("button", ImVec2(sizeWindow.x * 0.2f, sizeWindow.y * 0.2f))) {
                m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
            }
        
        }

        ImGui::End();

        // Display  
        renderHudEntities(target, states);

        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
        
    }
}