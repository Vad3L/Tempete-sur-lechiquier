#include "RulesEntity.h"

#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace tsl {

    namespace {
        constexpr std::size_t nb_page = 4;
    }

    RulesEntity::RulesEntity(gf::ResourceManager& resources, RulesChoice& choice)
    : m_font(resources.getFont("fonts/RifficFree-Bold.ttf"))
    , m_paper(resources.getTexture("images/paper.png"))
    , m_leftWidget(resources.getTexture("images/buttons/leftArrow.png"),resources.getTexture("images/buttons/leftArrow.png"),resources.getTexture("images/buttons/leftArrowSelected.png"))
    , m_rightWidget(resources.getTexture("images/buttons/rightArrow.png"),resources.getTexture("images/buttons/rightArrow.png"),resources.getTexture("images/buttons/rightArrowSelected.png"))
    , m_quitWidget(resources.getTexture("images/buttons/menuButton.png"),resources.getTexture("images/buttons/menuButton.png"),resources.getTexture("images/buttons/menuButtonSelected.png"))
    , m_numPage(0)
    {
        m_paper.setSmooth(true);

        m_leftWidget.setCallback([&choice, this]() { choice = RulesChoice::Before; if(m_numPage>=1) m_numPage--;});
        m_container.addWidget(m_leftWidget);

        m_rightWidget.setCallback([&choice, this]() { choice = RulesChoice::After; if(m_numPage<nb_page-1) m_numPage++;});
        m_container.addWidget(m_rightWidget);

        m_quitWidget.setCallback([&choice, this]() { choice = RulesChoice::Quit; m_numPage=0;});
        m_container.addWidget(m_quitWidget);

        m_rules[0] = "Ce jeu de cartes propose une variation simple et amusante du jeu d'échecs. Les joueurs utilisent un jeu d'échecs normal et complet, disposé de la manière habituelle.\n .\n Mise en Place \n .\nLe Premier joueur mélange des cartes, puis en distribue 5 à son adversaire et 5 à lui-même, une par une et faces cachées. Les cartes restantes, faces cachées, constituent la pioche. Chaque joueur examine ses cartes, et la partie commence. Le but du jeu reste de mettre le Roi adverse <mat>, mais les cartes permettent d'y parvenir par des moyens inhabituels. Chaque carte décrit un événement modifiant la structure ou le cours du jeu.";
        m_rules[1] = "Utilisation des Cartes \n . \n Le joueur qui joue une carte l'annonce clairement en rejetant celle-ci, face visible, sur la pile de carte jouer. L'effet décrit est aussitôt appliqué.\n Chaque joueur peut utiliser une carte, et une seule, à chaque coup (au sien et à celui de son adversaire).\n Chaque carte ne peut être utilisée qu'au moment précis indiqué en par la couleur du bandeau en dessous du titre de la carte.\n .\nJaune (rond): carte Avant coup\n Vert (carre): Après coup\n Bleu (losange): Avant - Après coup \nViolet (carre bord arrondi): Pendant tour adversaire\n";
        m_rules[2] = "Un coup peut être joué sans qu'aucune carte ne soit utilisée.\nLorsqu'un joueur vient d'abattre une carte, il en reprend aussitôt une sur le dessus de la pioche, de façon à avoir en permanence 5 cartes en main\n. \nAprès son coup, et à condition qu'il n'ait pas joué de carte à ce coup, un joueur peut se défausser d'une carte de sa main si cette carte ne peut pas être utilisée dans les conditions actuelles de la partie (par exemple Urbanisme si l'un des joueurs n'a plus de Tour sur l'échiquier ). Il prend aussitôt la première carte de la pioche pour la remplacer. \nLorsque la pioche est épuisée, les cartes de l'écart sont mélangées pour constituer une nouvelle pioche.";
        m_rules[3] = "Régle Fondamentale \n . \n Quelles que soient les situations bizarres créées par les cartes, la victoire ne peut être obtenue que par une position réelle de mat, et jamais par une simple <Prise du Roi>. Le déplacement créant la situation de mat peut très bien avoir été permis par le jeu d'une carte, mais la position elle-même doit être un mat.\n . \n Quelques précisions \n . \nLorsque le terme <Pièce> est utilisé dans le texte d'une carte, il désigne, l'ensemble des pièces du jeu, Dames, Rois et Pions inclus. Si l'effet d'une carte exclut certaines pièces, cela est précisé dans le texte de la carte.";
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
        const float min_scale = std::min(scale.x, scale.y);

        m_leftWidget.setPosition(coords.getRelativePoint({ 0.1f, 0.45f }));
        m_leftWidget.setScale(min_scale);
        m_leftWidget.setAnchor(gf::Anchor::Center);
        
        m_rightWidget.setScale(min_scale);
        m_rightWidget.setPosition(coords.getRelativePoint({ 0.9f, 0.45f }));
        m_rightWidget.setAnchor(gf::Anchor::Center);

        m_quitWidget.setScale(scale/2.5f);
        m_quitWidget.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
        m_quitWidget.setAnchor(gf::Anchor::Center);

        const unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.05f);
        const unsigned paragraphCharacterSize = coords.getRelativeCharacterSize(0.022f);
        const unsigned characterSize = coords.getRelativeCharacterSize(0.025f);

        gf::Sprite paper(m_paper);
        paper.setScale(scale);
        paper.setPosition(coords.getRelativePoint({ 0.5f, 0.05f }));
        paper.setAnchor(gf::Anchor::TopCenter);

        gf::Text gameRules("Règles du jeu", m_font , titleCharacterSize);	
        gameRules.setColor(gf::Color::White);
        gameRules.setScale(scale);
        gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.12f }));
        gameRules.setAnchor(gf::Anchor::Center);

        const float width = paper.getLocalBounds().getWidth();

        gf::Text paragraph(m_rules[m_numPage], m_font, paragraphCharacterSize);
        paragraph.setColor(gf::Color::Gray(0.3f));
        paragraph.setLineSpacing(1.5f);
        paragraph.setParagraphWidth(width-150);
        paragraph.setScale(scale);
        paragraph.setAlignment(gf::Alignment::Center);
        paragraph.setPosition(coords.getRelativePoint({ 0.5f, 0.46f }));
        paragraph.setAnchor(gf::Anchor::Center);

        char str[256];
        std::snprintf(str, sizeof(str), "%li/%li", m_numPage+1, nb_page);

        gf::Text numPage(str, m_font, characterSize);
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