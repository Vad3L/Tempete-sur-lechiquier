#include "RulesEntity.hpp"



RulesEntity::RulesEntity(gf::ResourceManager& resources)
: m_index(0)
, m_backgroundTexture(resources.getTexture("images/StartMenu.png"))
, m_rulesFont(resources.getFont("fonts/RifficFree-Bold.ttf"))
, m_font(resources.getFont("fonts/Trajan-Color-Concept.otf"))

{

}

void RulesEntity::update([[maybe_unused]] gf::Time time) {
}

void RulesEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
	gf::Coordinates coords(target);

	float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 1.0f)).height;
	float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;
	unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.04f);
	//unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.02f);

	gf::Sprite background(m_backgroundTexture);
	background.setColor(gf::Color::Opaque(0.20f));
	background.setPosition(coords.getCenter());
	background.setAnchor(gf::Anchor::Center);
	background.setScale(backgroundScale);
	target.draw(background, states);

	float height = target.getSize().height/1.5;
	float width = target.getSize().width/2;

	gf::RectangleShape paper({width, height});
	paper.setAnchor(gf::Anchor::Center);
	paper.setPosition(coords.getRelativePoint({0.5f, 0.4f}));
	paper.setColor(gf::Color::fromRgba32(100,74,75,150));

	gf::Text gameRules("Regles du jeu", m_rulesFont, height/75+width/20);	
	gameRules.setColor(gf::Color::Yellow);
	gameRules.setPosition(coords.getRelativePoint({ 0.5f, 0.1f}));
	gameRules.setAnchor(gf::Anchor::TopCenter);
	gameRules.setOutlineColor(gf::Color::Black);
	gameRules.setOutlineThickness(titleCharacterSize/4.f);

	std::string rules[4];
	rules[0] = "Ce jeu de cartes propose une variation simple et amusante du jeu d'échecs. Les joueurs utilisent un jeu d'échecs normal et complet, disposé de la manière habituelle.\n .\n Mise en Place \n .\nLe Premier joueur mélange des cartes, puis en distribue 5 à son adversaire et 5 à lui-même, une par une et faces cachées. Les cartes restantes, faces cachées, constituent la pioche. Chaque joueur examine ses cartes, et la partie commence. Le but du jeu reste de mettre le Roi adverse <mat>, mais les cartes permettent d'y parvenir par des moyens inhabituels. Chaque carte décrit un événement modifiant la structure ou le cours du jeu.";
	rules[1] = "Utilisation des Cartes \n . \n Le joueur qui joue une carte l'annonce clairement en rejetant celle-ci, face visible, sur la pile de carte jouer. L'effet décrit est aussitôt appliqué.\n Chaque joueur peut utiliser une carte, et une seule, à chaque coup (au sien et à celui de son adversaire).\n Chaque carte ne peut être utilisée qu'au moment précis indiqué en par la couleur du bandeau en dessous du titre de la carte.\n .\nJaune (rond): carte Avant coup\n Vert (carre): Après coup\n Bleu (losange): Avant - Après coup \nViolet (carre bord arrondi): Pendant tour adversaire\n.\n Un coup peut être joué sans qu'aucune carte ne soit utilisée.\nL'orsqu'un joueur vient d'abattre une carte, il en reprend aussitôt une sur le dessus de la pioche, de façon à avoir en permanence 5 cartes en main. ";
	rules[2] = "Après son coup, et à condition qu'il n'ait pas joué de carte à ce coup, un joueur peut se défausser d'une carte de sa main si cette carte ne peut pas être utilisée dans les conditions actuelles de la partie (par exemple Urbanisme si l'un des joueurs n'a plus de Tour sur l'échiquier ). Il prend aussitôt la première carte de la pioche pour la remplacer. \nLorsque la pioche est épuisée, les cartes de l'écart sont mélangées pour constituer une nouvelle pioche.";
	rules[3] = "Régle Fondamentale \n . \n Quelles que soient les situations bizarres créées par les cartes, la victoire ne peut être obtenue que par une position réelle de mat, et jamais par une simple <Prise du Roi>. Le déplacement créant la situation de mat peut très bien avoir été permis par le jeu d'une carte, mais la position elle-même doit être un mat. (Rappel : un Roi est mat lorsque la position est telle que le joueur ne peut empêcher qu'il soit capturé par l'adversaire au coup suivant.\n . \n Quelques précisions \n . \nLorsque le terme <Pièce> est utilisé dans le texte d'une carte, il désigne, l'ensemble des pièces du jeu, Dames, Rois et Pions inclus. Si l'effet d'une carte exclut certaines pièces, cela est précisé dans le texte de la carte.";

	gf::Text paragraph;
	paragraph.setColor(gf::Color::White);
	paragraph.setLineSpacing(1.5f);
	paragraph.setFont(m_rulesFont);
	paragraph.setCharacterSize((height/90+width/90));
	paragraph.setParagraphWidth(width-50);
	paragraph.setAlignment(gf::Alignment::Center);
	paragraph.setString(rules[m_index]);
	paragraph.setPosition(coords.getRelativePoint({ 0.5f, 0.45f}));
	paragraph.setAnchor(gf::Anchor::Center);

	target.draw(paper, states);
	target.draw(gameRules, states);
	target.draw(paragraph);
}