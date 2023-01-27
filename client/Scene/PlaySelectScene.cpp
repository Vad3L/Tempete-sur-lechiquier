#include "MenuScene.hpp"
#include "../GameHub.hpp"

#include "../Singletons.hpp"

PlaySelectScene::PlaySelectScene(GameHub& game, Network &network)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_network(network)
, m_rightAction("RightAction")
, m_leftAction("LeftAction")
, m_triggerAction("TriggerAction")
, m_quitAction("QuitAction")
, m_fullscreenAction("FullscreenAction")
, m_connectAction("ConnectAction")
, m_PlayTitleEntity(game.resources)
, m_ipWidget("pas de serveur", game.resources.getFont("fonts/Trajan-Color-Concept.otf"))
, m_index(0)
, m_leftWidget(game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrow.png"),game.resources.getTexture("images/button/leftArrowSelected.png"))
, m_rightWidget(game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrow.png"),game.resources.getTexture("images/button/rightArrowSelected.png"))
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
	
	btnClicked.setBuffer(gAudioManager().getSound("sounds/ClickButton.ogg"));
	btnClicked.setVolume(FxsVolume);

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

	auto setupButtonSprite = [&] (gf::SpriteWidget& button, auto callback) {
		button.setAnchor(gf::Anchor::Center);
		button.setCallback(callback);
		m_widgets.addWidget(button);
	};

	setupButton(m_ipWidget, [&] () {
		btnClicked.play();

		std::string ip = std::string(m_listIp[m_index].first);
		m_network.connect(ip,"43771");
		gf::Log::debug("Tentative de connexion  à %s\n", ip.c_str());

		gf::sleep(gf::milliseconds(500));
		
		if(m_network.isConnected()){
			gf::Log::info("Connexion réussie\n");
			m_game.replaceAllScenes(*m_game.game);
		}else{
			gf::Log::info("Connexions échoué\n");
			m_PlayTitleEntity.m_errorText.setString("Connexion au serveur a échoué");
		}
		
	});

	setupButtonSprite(m_rightWidget, [&] () {
		btnClicked.play();
		changeRightLeft(true);
	});

	setupButtonSprite(m_leftWidget, [&] () {
		btnClicked.play();
		changeRightLeft(false);
	});
	
  
}

void PlaySelectScene::doHandleActions([[maybe_unused]] gf::Window& window) {
	if (!isActive()) {
		return;
	}

	if(m_leftAction.isActive()) {
		m_leftWidget.triggerCallback();
		while(m_leftWidget.isDefault()){
			m_widgets.selectPreviousWidget();
		}
	}

	if(m_rightAction.isActive()) {
		m_rightWidget.triggerCallback();
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
		default :
			break;
	}
}

void PlaySelectScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
	constexpr float characterSize = 0.075f;
	constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);

	target.setView(getHudView());
	gf::Coordinates coords(target);

	const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
	const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
	const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);
	
	m_ipWidget.setCharacterSize(resumeCharacterSize);
	m_ipWidget.setPosition(coords.getRelativePoint({0.275f, 0.425f +characterSize }));
	m_ipWidget.setParagraphWidth(paragraphWidth);
	m_ipWidget.setPadding(paddingSize);

	m_leftWidget.setPosition(coords.getRelativePoint({( m_index != 0 ? 0.125f : 2.775f), 0.425f +characterSize}));
	m_leftWidget.setScale(1.f/2.f);

	m_rightWidget.setPosition(coords.getRelativePoint({ (m_index != m_listIp.size()-1 ? 0.875f : 2.775f), 0.425f +characterSize}));
	m_rightWidget.setScale(1.f/2.f);

	m_PlayTitleEntity.render(target,states);
	m_widgets.render(target, states);
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
		if(m_index >= 1){
			m_ipWidget.setString(m_listIp[m_index-1].second);
			m_index--;
		}
	}
	
}

void PlaySelectScene::onActivityChange(bool active) {
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

		btnClicked.setVolume(FxsVolume);
	}
}