#include "GameHub.h"

#include "../common/Constants.h"

#include <imgui.h>
#include <imgui_impl_gf.h>
#include <gf/Sleep.h>

namespace tsl {

	GameHub::GameHub()
	: gf::GameManager("Tempête sur l'échiquier", { GAME_DATADIR })
	, download(std::make_unique<DownloadScene>(*this))
	, m_loadingFinished(false)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
			
		io.IniFilename = nullptr;

		io.Fonts->AddFontFromFileTTF(resources.getAbsolutePath("fonts/RifficFree-Bold.ttf").string().c_str(), 36);
		
		ImGui_ImplGF_Init(getWindow(), getRenderer());

		getWindow().setVerticalSyncEnabled(true);
		getWindow().setFramerateLimit(120);
	}

	GameHub::~GameHub() {
		ImGui_ImplGF_Shutdown();
    	ImGui::DestroyContext();
	}

	void GameHub::loadingOtherAssets(Network& network) {
		
		common = std::make_unique<CommonScene>(*this);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();

		start = std::make_unique<StartScene>(*this);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();
		
		menu = std::make_unique<MenuScene>(*this);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();

		connection = std::make_unique<ConnectionScene>(*this, network);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();

		rules = std::make_unique<RulesScene>(*this);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();

		settings = std::make_unique<SettingsScene>(*this);
		gf::sleep(gf::seconds(0.2f));
		download->changeFrame();

		gf::Log::info("Download finished !!!\n");
		
		m_loadingFinished = true;
	}

	bool GameHub::loadingFinished() {
		return m_loadingFinished;
	}

}