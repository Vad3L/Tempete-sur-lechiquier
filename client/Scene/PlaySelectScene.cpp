#include "MenuScene.hpp"

#include <gf/Log.h>
#include <gf/Coordinates.h>
#include "../GameHub.hpp"


PlaySelectScene::PlaySelectScene(GameHub& game)
: gf::Scene(game.getRenderer().getSize())
, m_game(game)
, m_upAction("UpAction")
, m_downAction("DownAction")
, m_triggerAction("TriggerAction")
, m_quitAction("Quit")
, m_PlayTitleEntity(game.resources)
, m_hostGame("Host", game.resources.getFont("Trajan-Color-Concept.otf"))
, m_join("Join", game.resources.getFont("Trajan-Color-Concept.otf"))
{
    setClearColor(gf::Color::Black);
    addHudEntity(m_PlayTitleEntity);

    m_quitAction.addKeycodeKeyControl(gf::Keycode::Escape);
    addAction(m_quitAction);

    m_upAction.addScancodeKeyControl(gf::Scancode::Up);
    addAction(m_upAction);

    m_downAction.addScancodeKeyControl(gf::Scancode::Down);
    addAction(m_downAction);

    m_triggerAction.addScancodeKeyControl(gf::Scancode::Return);
    m_triggerAction.addMouseButtonControl(gf::MouseButton::Left);
    addAction(m_triggerAction);

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

    setupButton(m_hostGame, [&] () {
        gf::Log::debug("host button pressed!\n");
        /*m_game.state.levelIndex = 0;
        m_game.level.loadLevel(m_game.data, m_game.state.levelIndex);
        m_game.replaceAllScenes(m_game.level);*/
    });

    setupButton(m_join, [&] () {
        gf::Log::debug("join button pressed!\n");
        m_game.replaceAllScenes(m_game.game);
    });
    }

    void PlaySelectScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
        return;
    }

    if (m_upAction.isActive()) {
        m_widgets.selectPreviousWidget();
    }

    if (m_downAction.isActive()) {
        m_widgets.selectNextWidget();
    }

    if (m_triggerAction.isActive()) {
        m_widgets.triggerAction();
    }

    if (m_quitAction.isActive()) {
        m_game.replaceScene(m_game.start);
    }
    }

    void PlaySelectScene::doProcessEvent(gf::Event& event) {
        switch (event.type)
        {
            case gf::EventType::MouseMoved:
            m_widgets.pointTo(m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getHudView()));
            break;
        }
    }

    void PlaySelectScene::doRender(gf::RenderTarget& target, const gf::RenderStates &states) {
        constexpr float characterSize = 0.075f;
        constexpr float spaceBetweenButton = 0.050f;
        constexpr gf::Vector2f backgroundSize(0.5f, 0.3f);

        target.setView(getHudView());
        gf::Coordinates coords(target);

        const float paragraphWidth = coords.getRelativeSize(backgroundSize - 0.05f).x;
        const float paddingSize = coords.getRelativeSize({0.01f, 0.f}).x;
        const unsigned resumeCharacterSize = coords.getRelativeCharacterSize(characterSize);

        m_hostGame.setCharacterSize(resumeCharacterSize);
        m_hostGame.setPosition(coords.getRelativePoint({0.275f, 0.425f}));
        m_hostGame.setParagraphWidth(paragraphWidth);
        m_hostGame.setPadding(paddingSize);

        m_join.setCharacterSize(resumeCharacterSize);
        m_join.setPosition(coords.getRelativePoint({0.275f, 0.425f + characterSize + spaceBetweenButton}));
        m_join.setParagraphWidth(paragraphWidth);
        m_join.setPadding(paddingSize);

        m_widgets.render(target, states);
        m_PlayTitleEntity.render(target,states);
    }

    void PlaySelectScene::doShow() {
    m_widgets.clear();

    m_hostGame.setDefault();
    m_widgets.addWidget(m_hostGame);

    m_join.setDefault();
    m_widgets.addWidget(m_join);

    m_widgets.selectNextWidget();
}



