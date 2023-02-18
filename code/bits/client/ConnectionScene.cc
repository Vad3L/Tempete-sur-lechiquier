#include "ConnectionScene.h"

#include "GameHub.h"
#include "../common/ImGuiConstants.h"

#include <gf/Coordinates.h>

#include <imgui.h>
#include <imgui_impl_gf.h>

namespace tsl {
    
    ConnectionScene::ConnectionScene(GameHub& game, Network &network)
    : gf::Scene({game.getRenderer().getSize()})
    , m_game(game)
    , m_network(network)
    , m_connectionAsked(false)
    {
        setClearColor(gf::Color::Black);

        std::strncpy(m_hostnameBuffer.data(), "", 255);
        std::strncpy(m_nameBuffer.data(), "", 255);
    }

    void ConnectionScene::doProcessEvent(gf::Event& event) {
        ImGui_ImplGF_ProcessEvent(event);
    }

    void ConnectionScene::doUpdate(gf::Time time) {
        ImGui_ImplGF_Update(time);
    }

    void ConnectionScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const auto position = coords.getCenter();
        
        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        
        if (ImGui::Begin("Connexion", nullptr, DefaultWindowFlags)) {
            if (m_network.isConnecting()) {
                ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Connexion...");

                if (m_network.isConnected()) {
                    m_connectionAsked = false;
                    //m_scenes.replaceScene(*m_scenes.lobby, m_scenes.fadeEffect, gf::seconds(0.4f));

                    //ClientHello data;
                    //data.name = std::string(m_nameBuffer.data());
                    //m_network.send(data);
                }
            } else {
                ImGui::Text("Nom d' hôte :");
                ImGui::SameLine();
                float x = ImGui::GetCursorPosX();
                
                ImGui::InputText("###nom d'hôte", m_hostnameBuffer.data(), m_hostnameBuffer.size());

                ImGui::Text("Nom :");
                ImGui::SameLine();
                ImGui::SetCursorPosX(x);
                ImGui::InputText("###nom", m_nameBuffer.data(), m_nameBuffer.size());
                ImGui::SetItemDefaultFocus();

                ImGui::Indent();
                ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - DefaultButtonSize.x);

                if (ImGui::Button("Retour", DefaultButtonSize)) {
                    m_connectionAsked = false;
                    m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
                }

                ImGui::SameLine();

                if (ImGui::Button("Connexion", DefaultButtonSize)) {
                    m_network.connect(std::string(m_hostnameBuffer.data()));
                    m_connectionAsked = true;
                }

                if (m_connectionAsked) {
                    const char* text = "Erreur: impossible de se connecter au serveur.";
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(text).x) * 0.5f);
                    ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s", text);
                }
            }
        }

        ImGui::End();

        // Display  
        renderHudEntities(target, states);

        ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 20.f;
		style.Colors[ImGuiCol_Text]                  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
		style.Colors[ImGuiCol_TitleBg]         		 = ImVec4(0.0f, 0.0f, 0.0f, 0.74f);
		style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0f, 0.0f, 0.0f, 0.74f);
		style.Colors[ImGuiCol_Button]                = ImVec4(0.5f, 0.5f, 0.5f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.5f, 0.5f, 0.5f, 0.84f);
		style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
        
        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }
}