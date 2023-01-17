#include "Deck.hpp"

Deck::Deck(){
    //format du fichier
    //NUM;NAME;DESCRIPTION;TURN;ACTION;EFFECT (3 dernières valeurs sont des entier correspondant à la la valeur dans leur énum)
    for(int k =0 ;k<5;k++) {
        std::ifstream file(std::string(CARDS_DESCRIPTIONDIR)+"descriptionCards.txt");

        std::string delimiter = ";";

        if (!file) {
            gf::Log::error("Impossible d'ouvrir le fichier.\n");
        }else{
            std::string line;
        
            while (std::getline(file, line)) {
                std::string tab[6];
                for(int i=0 ; i<6 ; i++) {
                    
                    std::size_t index = line.find(delimiter);
                    tab[i] = line.substr(0, index);
                    
                    line = line.substr(index+1);
                }

                gf::Log::info("card /%s/,/%s/,/%s/,/%s/,/%s/,/%s/\n", tab[0].c_str(), tab[1].c_str(), tab[2].substr(0,50).c_str(), tab[3].c_str(), tab[4].c_str(), tab[5].c_str());
                //tab[0] num
                //tab[1] name
                //tab[2] description
                //tab[3] num turn
                //tab[4] num action
                //tab[5] num effect
                int num = atoi(tab[0].c_str());
                int numTurn = atoi(tab[3].c_str());
                int numAction = atoi(tab[4].c_str());
                int numEffect = atoi(tab[5].c_str());
                Turn turn = (Turn)numTurn;
                Action action = (Action)numAction;
                Effect effect = (Effect)numEffect;

                Card c(num, tab[1], tab[2], turn, action, effect);
                m_deck.push_back(c);
            }
            file.close();
        }
    }

    shuffle();
    //displayDeck();
}


Card Deck::getFirst() {
    Card tmp=m_deck.back();
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
    int i=0;
    srand((unsigned int)time(0));
    int index;
    int indexbis;
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