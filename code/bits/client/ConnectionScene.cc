#include "ConnectionScene.h"

#include "GameHub.h"
#include "../common/ImGuiConstants.h"
#include "Tools.h"

#include <gf/Coordinates.h>
#include <imgui.h>
#include <imgui_impl_gf.h>

namespace tsl {
    
    ConnectionScene::ConnectionScene(GameHub& game, Network &network)
    : gf::Scene({game.getRenderer().getSize()})
    , m_game(game)
    , m_network(network)
    , m_model(m_game.m_model)
    , m_connectionAsked(false)
    , m_choice(ConnectionChoice::None)
    , m_connection(m_game.resources, m_choice, m_game.m_model)
    {
        setClearColor(gf::Color::Black);

        std::strncpy(m_hostnameBuffer.data(), "localhost", 255);

        addHudEntity(m_connection);
    }

    void ConnectionScene::doProcessEvent(gf::Event& event) {
        ImGui_ImplGF_ProcessEvent(event);

        switch (event.type) {
            case gf::EventType::MouseMoved:
                m_connection.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
                break;

            case gf::EventType::MouseButtonPressed:
                m_connection.pointTo(m_game.getRenderer().mapPixelToCoords(event.mouseButton.coords));
                m_connection.triggerAction();
                break;

            default:
                break;
        }
    }

    void ConnectionScene::doUpdate(gf::Time time) {
        if (!isActive()) {
			return;
		}

        ImGui_ImplGF_Update(time);

        switch (m_choice) {
            case ConnectionChoice::None:
                return;
            case ConnectionChoice::Connection:
                m_network.connect(std::string(m_hostnameBuffer.data()));
                m_connectionAsked = true;
                break;
            case ConnectionChoice::Quit:
                m_game.replaceScene(*m_game.menu, m_game.blackoutEffect, gf::seconds(0.4f));
                break;
        }

        playClickButton();
        m_choice = ConnectionChoice::None;
    }

    void ConnectionScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);
        const auto position = coords.getRelativePoint({ 0.5f, 0.6f });
        
        ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_Text]                  = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);;
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.9f, 0.8f, 0.68f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.9f, 0.8f, 0.68f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.9f, 0.8f, 0.68f, 1.0f);

        // UI
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y), 0, ImVec2(0.5f, 0.5f));
        
        const ImVec2 sizeWindow = ImVec2(ImGui::GetWindowSize().x * scale.x * 1.1f, ImGui::GetWindowSize().y * scale.y * 1.2f);
        ImGui::SetNextWindowSize(sizeWindow);

        if (ImGui::Begin("Connexion", nullptr, DefaultWindowFlags|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground)) {
            ImGui::SetWindowFontScale(coords.getRelativeCharacterSize(0.02f) * 0.06f);
            
            if (m_network.isConnecting()) {
                std::string text = m_model.getWord("Connecting") + "...";
                ImGui::SetCursorPosX((sizeWindow.x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f);
                ImGui::SetCursorPosY(sizeWindow.y * 0.4f);

                ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "%s", text.c_str());

                if (m_network.isConnected()) {
                    m_connectionAsked = false;
                    m_game.replaceScene(*m_game.waiting, m_game.blackoutEffect, gf::seconds(0.4f));
                }
            } else {
                ImGui::PushItemWidth(sizeWindow.x * 0.7f);
                ImGui::SetCursorPosY(sizeWindow.y * 0.245f);

                ImGui::InputText("###hostname", m_hostnameBuffer.data(), m_hostnameBuffer.size());

                if (m_connectionAsked) {
                    std::string text = m_model.getWord("Error: unable to connect to the server.");
                    int scatter = 0.46f * text.size();
                    text = "\t" + text.substr(0,scatter)+ '\n' + text.substr(scatter);

                    ImGui::SetCursorPosX((sizeWindow.x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f);
                    ImGui::SetCursorPosY(sizeWindow.y * 0.4f);

                    ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "%s", text.c_str());
                }
            }
        }

        ImGui::End();

        // Display  
        renderHudEntities(target, states);
        
        ImGui::Render();
        ImGui_ImplGF_RenderDrawData(ImGui::GetDrawData());
    }
}