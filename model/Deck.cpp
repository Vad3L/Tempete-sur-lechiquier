#include "Deck.hpp"

Deck::Deck(std::vector<int> numCards){
	std::map<int, std::function<bool(Plateau&, std::vector<gf::Vector2i>)>> m_execsfuncs;
	std::map<int, std::function<bool(Plateau&, Phase)>> m_isplayfuncs;

	m_execsfuncs.insert({ 1, AmourCourtois });
	m_isplayfuncs.insert({ 1, AmourCourtoisIsPlayable });

	m_execsfuncs.insert({ 6, Apartheid });
	m_isplayfuncs.insert({ 6, ApartheidIsPlayable });
	
	m_execsfuncs.insert({ 7, Asile });
	m_isplayfuncs.insert({ 7, AsileIsPlayable });
	
	m_execsfuncs.insert({ 10, AvionPrive });
	m_isplayfuncs.insert({ 10, AvionPriveIsPlayable });

	m_execsfuncs.insert({ 15, BombeAtomique });
	m_isplayfuncs.insert({ 15, BombeAtomiqueIsPlayable });

	m_execsfuncs.insert({ 16, BonnesCopines });
	m_isplayfuncs.insert({ 16, BonnesCopinesIsPlayable });

	m_execsfuncs.insert({ 20, Box });
	m_isplayfuncs.insert({ 20, BoxIsPlayable });

	m_execsfuncs.insert({ 29, Chameau });	
	m_isplayfuncs.insert({ 29, ChameauIsPlayable });

	m_execsfuncs.insert({ 33, ChevalFou });
	m_isplayfuncs.insert({ 33, ChevalFouIsPlayable });
	
	m_execsfuncs.insert({ 45, Desintegration });
	m_isplayfuncs.insert({ 45, DesintegrationIsPlayable });

	m_execsfuncs.insert({ 104, Princess });
	m_isplayfuncs.insert({ 104, PrincessIsPlayable });

	m_execsfuncs.insert({ 110, QuatreCoin });
	m_isplayfuncs.insert({ 110, QuatreCoinIsPlayable });
	
	m_execsfuncs.insert({ 122, Schizophrenie });
	m_isplayfuncs.insert({ 122, SchizophrenieIsPlayable });
		
	m_execsfuncs.insert({ 126, TourFada });
	m_isplayfuncs.insert({ 126, TourFadaIsPlayable });

	m_execsfuncs.insert({ 131, Urbanisme });
	m_isplayfuncs.insert({ 131, UrbanismeIsPlayable });
	
	m_execsfuncs.insert({ 132, Vampirisme });
	m_isplayfuncs.insert({ 132, VampirismeIsPlayable });

	m_execsfuncs.insert({ 135, VisitesOfficielles });
	m_isplayfuncs.insert({ 135, VisitesOfficiellesIsPlayable });

	//format du fichier
	//NUM;NAME;DESCRIPTION;TURN;ACTION;EFFECT;nbCase (3 dernières valeurs sont des entier correspondant à la la valeur dans leur énum)
	int borne = 11;
	if(numCards.size()<1) {
		borne = 1;
	}
	for(int k =0 ;k<borne;k++) {
		std::ifstream file(std::string(CARDS_DESCRIPTIONDIR)+"descriptionCards.txt");

		std::string delimiter = ";";

		if (!file) {
			gf::Log::error("Impossible d'ouvrir le fichier.\n");
		} else {
			std::string line;
		
			while (std::getline(file, line)) {
				std::array<std::string,7> tab;
				for(std::size_t i=0 ; i<tab.size() ; i++) {
					
					std::size_t index = line.find(delimiter);
					tab[i] = line.substr(0, index);
					
					line = line.substr(index+1);
				}

				gf::Log::info("card /%s/,/%s/,/%s/,/%s/,/%s/,/%s/,/%s/\n", tab[0].c_str(), tab[1].c_str(), tab[2].substr(0,50).c_str(), tab[3].c_str(), tab[4].c_str(), tab[5].c_str(), tab[6].c_str());
				//tab[0] num
				//tab[1] name
				//tab[2] description
				//tab[3] num turn
				//tab[4] num action
				//tab[5] num effect
				//tab[6] nub click max possible
				int num = atoi(tab[0].c_str());
				int numTurn = atoi(tab[3].c_str());
				int numAction = atoi(tab[4].c_str());
				int numEffect = atoi(tab[5].c_str());
				int nbClickPossible = atoi(tab[6].c_str());
				Turn turn = (Turn)numTurn;
				Action action = (Action)numAction;
				Effect effect = (Effect)numEffect;

				Card c(num, tab[1], tab[2], turn, action, effect, nbClickPossible);

				c.m_isPlayable = m_isplayfuncs[num];
				c.m_execute = m_execsfuncs[num];
				
				if(!numCards.empty()) {
					if(std::find(numCards.begin(), numCards.end(), num) != numCards.end()) {
						m_deck.push_back(c);
					}
				}else {
					m_deck.push_back(c);
				}
			}
			file.close();
		}
	}

	shuffle();
	//displayDeck();
}


Card Deck::getFirst() {
	Card tmp = m_deck.back();
	m_deck.pop_back();
	return tmp;
}

void Deck::drop(Card m_card){
	m_discard.push_back(m_card);
}

void Deck::swap(Card &a, Card &b){
	std::swap(a, b);
}

void Deck::shuffle(){
	std::size_t i=0;
	srand((unsigned int)time(0));
	std::size_t index;
	std::size_t indexbis;
	while(i<m_deck.size()){
		index=rand()%m_deck.size();
		indexbis=rand()%m_deck.size();
		swap(m_deck.at(index),m_deck.at(indexbis));
		i++;
	}
}

void Deck::displayDeck() {

	for(Card &card : m_deck){
		std::cout << card.m_name << std::endl;
	}
}

std::pair<std::vector<Card>,std::vector<Card>> Deck::distribute(){
	std::vector<Card> playerOne;
	std::vector<Card> playerTwo;
	for(int i = 0 ; i < 10 ; ++i){
		if(i < 5){
			playerOne.push_back(getFirst());
		}else{
			playerTwo.push_back(getFirst());
		}
	}
	return std::make_pair(playerOne,playerTwo);
}

int Deck::getNbCardInDeck(){
	return (int)m_deck.size();
}

void Deck::emptyDiscard () {
	m_deck = m_discard;
	m_discard = std::vector<Card>();
	shuffle(); 
}
