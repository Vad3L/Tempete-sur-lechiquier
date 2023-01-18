#include "MenuScene.hpp"
#include "../GameHub.hpp"


PlaySelectScene::PlaySelectScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_rightAction("RightAction")
, m_leftAction("LeftAction")
, m_triggerAction("TriggerAction")
, m_quitAction("QuitAction")
, m_fullscreenAction("FullscreenAction")
, m_PlayTitleEntity(game.resources)
, m_ipWidget("pas de serveur", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_connectAction("ConnectAction")
, m_index(0)
, m_leftWidget("<", game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_rightWidget(">", game.resources.getFont("fonts/DejaVuSans.ttf"))
, m_listIp()
{
	setClearColor(gf::Color::Black);
	addHudEntity(m_PlayTitleEntity);

	m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
	addAction(m_fullscreenAction);
	
	m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
	addAction(m_quitAction);

	m_leftAction.addScancodeKeyControl(gf::Scancode::Left);
	addAction(m_leftAction);

	m_rightAction.addScancodeKeyControl(gf::Scancode::Right);
	addAction(m_rightAction);

	m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
	addAction(m_triggerAction);

	m_connectAction.addScancodeKeyControl(gf::Scancode::Return);
	addAction(m_connectAction);
	
	auto setupButton = [&] (gf::TextButtonWidget& button, auto callback) {
		button.setDefaultTextColor(gf::Color::Black);
		button.setDefaultBackgroundColor(gf::Color::Gray(0.7f));
		button.setSelectedTextColor(gf::Color::Black);
		button.setSelectedBackgroundColor(gf::Color::fromRgba32(212,30,27,255));
		button.setDisabledTextColor(gf::Color::Black);
		button.setDisabledBackgroundColor(gf::Color::Red);
		button.setAnchor(gf::Anchor::TopLeft);
		button.setAlignment(gf::Alignment::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_ipWidget, [&] () {
		std::string ip = std::string(m_listIp[m_index].first);
		m_game.m_network.connect(ip,"43771");
		gf::Log::debug("Tentative de connexion  à %s\n", ip.c_str());

		gf::sleep(gf::milliseconds(500));
		
		if(m_game.m_network.isConnected()){
			gf::Log::debug("Connexion réussie\n");
			m_game.replaceAllScenes(*m_game.game);
		}else{
			gf::Log::debug("Connexions échoué\n");
			m_PlayTitleEntity.m_errorText.setString("Connexion au serveur a échoué");
		}
		
	});

	setupButton(m_rightWidget, [&] () {
		changeRightLeft(true);
	});

	setupButton(m_leftWidget, [&] () {
		changeRightLeft(false);
	});
	
  
}

void PlaySelectScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if(m_leftAction.isActive()) {
		changeRightLeft(false);
		while(m_leftWidget.isDefault()){
			m_widgets.selectPreviousWidget();
		}
	}

	if(m_rightAction.isActive()) {
		changeRightLeft(true);
		while(m_rightWidget.isDefault()){
			m_widgets.selectNextWidget();
		}
	}

	if (m_fullscreenAction.isActive()) {
		window.toggleFullscreen();
	}

	if (m_triggerAction.isActive()) {
		m_widgets.triggerAction();
	}

	if(m_connectAction.isActive()) {
		m_ipWidget.triggerCallback();
	}

	if (m_quitAction.isActive()) {
		m_game.replaceScene(*m_game.start);
	}
}

void PlaySelectScene::doProcessEvent(gf::Event& event) {
	switch (event.type){
		case gf::EventType::MouseMoved:
			m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
			break;
	}
}

void PlaySelectScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	constexpr float characterSize = 0.075f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);
	constexpr gf::Vector2f backgroundSizeArrow(0.2f, 0.3f);

	target.setView(getHudView());
	gf::Coordinates coords(target);

	const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paragraphWidthArrow = coords.getRelativeSize(backgroundSizeArrow - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);
	
	m_ipWidget.setCharacterSize(resumeCharacterSize);
	m_ipWidget.setPosition(coords.getRelativePoint({0.275f, 0.425f + characterSize}));
	m_ipWidget.setParagraphWidth(paragraphWidth);
	m_ipWidget.setPadding(paddingSize);

	if(m_index != 0 ){
		m_leftWidget.setCharacterSize(resumeCharacterSize);
		m_leftWidget.setPosition(coords.getRelativePoint({0.075f, 0.425f + characterSize }));
		m_leftWidget.setParagraphWidth(paragraphWidthArrow);
		m_leftWidget.setPadding(paddingSize);
	}else{
		m_leftWidget.setPosition(coords.getRelativePoint({2.775, 0.425f + characterSize }));
	}


	if(m_index != m_listIp.size()-1 ){
		m_rightWidget.setCharacterSize(resumeCharacterSize);
		m_rightWidget.setPosition(coords.getRelativePoint({0.775, 0.425f + characterSize }));
		m_rightWidget.setParagraphWidth(paragraphWidthArrow);
		m_rightWidget.setPadding(paddingSize);
	}else{
		m_rightWidget.setPosition(coords.getRelativePoint({2.775, 0.425f + characterSize }));
	}
	m_widgets.render(target, states);
	m_PlayTitleEntity.render(target,states);
}

void PlaySelectScene::doShow() {
	m_widgets.clear();

	m_ipWidget.setDefault();
	m_leftWidget.setDefault();
	m_rightWidget.setDefault();
	
	m_widgets.addWidget(m_ipWidget);
	m_widgets.addWidget(m_leftWidget);
	m_widgets.addWidget(m_rightWidget);

	//m_widgets.selectNextWidget();
}

void PlaySelectScene::changeRightLeft(bool value) {
	//true right
	m_PlayTitleEntity.m_errorText.setString(" ");
	if(value){
		if(m_index+1 < m_listIp.size()){
			m_ipWidget.setString(m_listIp[m_index+1].second);
			m_index++;
		}
	}else{
		if(m_index-1 >= 0){
			m_ipWidget.setString(m_listIp[m_index-1].second);
			m_index--;
		}
	}
	
}

void PlaySelectScene::onActivityChange(bool active){
	if(active){
		m_PlayTitleEntity.m_errorText.setString(" ");
		m_index = 0;
		gf::Log::debug("Scene active\n");
		std::ifstream file(std::string(GAME_CONFIGDIR)+"IpList.txt");

		std::string delimiter = ";";

		if (!file) {
			gf::Log::error("Impossible d'ouvrir le fichier.\n");
		}else{
			std::string line;
			m_listIp.clear();
		
			std::regex pattern(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d).?\b){4}$)");
			std::size_t i = 0;
			
			while (std::getline(file, line)) {
				std::size_t index = line.find(delimiter);
				std::string nameServer = line.substr(0, index);
				std::string ip = line.substr(index+1);
				if(std::regex_search(ip, pattern)) {
					m_listIp.insert({i, std::make_pair(ip, nameServer)});
					i++;
				}
			}
			
			if(m_listIp.size()>0) {
				m_ipWidget.setString(m_listIp[0].second);
			}
			
			file.close();
		}
	}
}