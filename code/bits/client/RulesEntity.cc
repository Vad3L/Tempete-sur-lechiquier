#include "RulesEntity.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <boost/algorithm/string/replace.hpp>

namespace tsl {

    namespace {
        constexpr std::size_t nb_page = 4;
        std::array<std::string, 4> rules;
    }

    RulesEntity::RulesEntity(gf::ResourceManager& resources, RulesChoice& choice, GameModel& model)
    : m_model(model)
    , m_font(resources.getFont("fonts/RifficFree-Bold.ttf"))
    , m_paper(resources.getTexture("images/Paper.png"))
    , m_leftWidget(resources.getTexture("images/buttons/LeftArrow.png"),resources.getTexture("images/buttons/LeftArrow.png"),resources.getTexture("images/buttons/LeftArrowSelected.png"))
    , m_rightWidget(resources.getTexture("images/buttons/RightArrow.png"),resources.getTexture("images/buttons/RightArrow.png"),resources.getTexture("images/buttons/RightArrowSelected.png"))
    , m_quitWidget(resources.getTexture("images/buttons/Close.png"),resources.getTexture("images/buttons/Close.png"),resources.getTexture("images/buttons/CloseSelected.png"))
    , m_numPage(0)
    {
        m_paper.setSmooth(true);

        m_leftWidget.setCallback([&choice, this]() { choice = RulesChoice::Before; if(m_numPage>=1) m_numPage--;});
        m_container.addWidget(m_leftWidget);

        m_rightWidget.setCallback([&choice, this]() { choice = RulesChoice::After; if(m_numPage<nb_page-1) m_numPage++;});
        m_container.addWidget(m_rightWidget);

        m_quitWidget.setCallback([&choice, this]() { choice = RulesChoice::Quit; m_numPage=0;});
        m_container.addWidget(m_quitWidget);

        std::ifstream file("../data/Rules.csv");

        if(!file) {
            gf::Log::error("Failed to open file.\n");
        }
        else {
            std::string line;
            std::size_t cpt = 1;

            while (std::getline(file, line)) {
                std::array<std::string, languages.size()> tab;
                for(std::size_t i=0 ; i<tab.size() ; i++) {
            
                    std::size_t index = line.find(";");
                    tab[i] = line.substr(0, index);
                    
                    line = line.substr(index+1);
                }

                std::string str = "Rules" + std::to_string(cpt);

                boost::replace_all(tab[0], "@", "\n");
                boost::replace_all(tab[1], "@", "\n");
                boost::replace_all(tab[2], "@", "\n");

                // English
                m_model.dictionary[languages[0]].insert(std::make_pair(str, tab[0]));
                // French
                m_model.dictionary[languages[1]].insert(std::make_pair(str, tab[1]));
                // Spanish
                model.dictionary[languages[2]].insert(std::make_pair(str, tab[2]));

                cpt++;
            }
            file.close();
        }
    }

    void RulesEntity::pointTo(gf::Vector2f coords) {
        m_container.pointTo(coords);
    }

    void RulesEntity::triggerAction() {
        m_container.triggerAction();
    }
    
    void RulesEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        gf::Coordinates coords(target);
        const gf::Vector2f scale = coords.getWindowSize() / gf::vec(1920.0f, 1080.0f);

        m_leftWidget.setPosition(coords.getRelativePoint({ 0.1f, 0.45f }));
        m_leftWidget.setScale(scale/1.25f);
        m_leftWidget.setAnchor(gf::Anchor::Center);
        
        m_rightWidget.setScale(scale/1.25f);
        m_rightWidget.setPosition(coords.getRelativePoint({ 0.9f, 0.45f }));
        m_rightWidget.setAnchor(gf::Anchor::Center);

        m_quitWidget.setScale(scale);
        m_quitWidget.setPosition(coords.getRelativePoint({ 0.79f, 0.12f }));
        m_quitWidget.setAnchor(gf::Anchor::Center);

        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.05f }));
        paper.setAnchor(gf::Anchor::TopCenter);

        gf::Text gameRules(m_model.getWord("Games Rules"), m_font, 50);	
        gameRules.setColor(gf::Color::White);
        gameRules.setScale(scale);
        gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.12f }));
        gameRules.setAnchor(gf::Anchor::Center);

        const float width = paper.getLocalBounds().getWidth();

        gf::Text paragraph(m_model.getWord("Rules" + std::to_string(m_numPage+1)), m_font);
        paragraph.setColor(gf::Color::Gray(0.3f));
        paragraph.setLineSpacing(1.5f);
        paragraph.setParagraphWidth(width);
        paragraph.setScale(scale * 0.74f);
        paragraph.setAlignment(gf::Alignment::Center);
        paragraph.setPosition(coords.getRelativePoint({ 0.5f, 0.46f }));
        paragraph.setAnchor(gf::Anchor::Center);

        gf::Text numPage(std::to_string(m_numPage+1) + "/" + std::to_string(nb_page), m_font);
        numPage.setColor(gf::Color::Blue);
        numPage.setScale(scale);
        numPage.setPosition(coords.getRelativePoint({ 0.76f, 0.72f }));
        numPage.setAnchor(gf::Anchor::Center);

        target.draw(paper, states);
        target.draw(gameRules, states);
        target.draw(paragraph, states);
        target.draw(numPage, states);
        m_container.render(target, states);	
    }

}