#include "Deck.hpp"

Deck::Deck(){

    //Create cards
    Card c1("Chameau","Vous transformez l'un de vos Cavaliers en Chameau, et cela définitivement. Le Chameau, comme le Cavalier, peut se déplacer en sautant par-dessus les cases occupées. Mais alors que le Cavalier saute dans l'angle opposé d'un rectangle 3-2, le Chameau saute dans l'angle opposé d'un rectangle 4-3. Schéma : déplacements du Chameau.",Turn::AFTER,Effect::CHOOSECASE);
    Card c2("Coup d'etat","Votre Roi devient un simple Prince. Il continue à se déplacer comme auparavant, mais n'est désormais qu'une pièce ordinaire,pouvant être prise. Une autre pièce (sauf Dame ou Tour !), qui conserve également toutes ses capacités de déplacement, s'empare du pouvoir. C'est cette nouvelle pièce <<Royale>> que votre adversaire devra mater",Turn::AFTER,Effect::NONE);
    Card c3("Pas question !","Cette carte annule toute autre carte.",Turn::DURING,Effect::CANCELCARD);
    Card c4("Niet.","Cette carte annule toute autre carte.",Turn::DURING,Effect::CANCELCARD);
    Card c5("Charge","Avancez tous ceux de vos Pions que vous voulez et qui le peuvent d'une case.",Turn::REPLACE,Effect::NONE);
    Card c6("Banzai","Vous avancez un de vos Pions de 3 cases (aucune prise en passant ne sera possible).",Turn::REPLACE,Effect::NONE);
    Card c7("Amphetamines","Le fou que vous venez de déplacer peut rejouer à nouveau, tout de suite, s'il ne vient pas de prendre une pièce adverse.", Turn::AFTER,Effect::NONE);
    Card c8("Favori","Vous pouvez à ce coup déplacer n'importe laquelle de vos pièces (sauf un pion) à la manière d'une Dame, mais vous ne pouvez pas ainsi prendre une pièce adverse.",Turn::REPLACE,Effect::NONE);
    Card c9("Bombe Atomique","Si la pièce (ou le Pion) que vous venez de déplacer n'a pas pris une pièce adverse, vous pouvez faire \"exploser\" cette pièce. Elle est alors retirée du jeu, ainsi que toutes les autres pièces, quelles que soient leur couleur, qui se trouvent sur les huit case adjacentes. Les Rois ne sont pas affectés par les explosions nucléaires.",Turn::AFTER,Effect::NONE);
    Card c10("Piège","En jouant cette carte, vous séléctionnez une case sur l'échiquier vide ou occupée par une de vos pièces. Dès qu'une pièce adverse s'arrête sur cette case, elle tombe dans une trappe et est retirée du jeu (s'il s'agit du Roi, la trappe est actionnée, mais le Roi reste sur la case).",Turn::AFTER,Effect::CHOOSECASE);
    Card c11("Quatre coins","Si trois des quatres coins de l'échiquier sont occupés, vous pouvez amener une de vos pièces, de votre choix, dans le quatrième coin",Turn::REPLACE,Effect::CHOOSECASE);
    Card c12("Exil","Vous remettez sur la case, ou sur l'une des cases où elle pouvait se trouver en début de partie, une pièce adverse de votre choix.",Turn::AFTER,Effect::CHOOSECASE);

    c1.m_execute = Chameau;
    c11.m_execute = QuatreCoin;
    //We add cards in deck
    m_deck.push_back(c1);
    m_deck.push_back(c2);
    m_deck.push_back(c3);
    m_deck.push_back(c4);
    m_deck.push_back(c5);
    m_deck.push_back(c6);
    m_deck.push_back(c7);
    m_deck.push_back(c8);
    m_deck.push_back(c9);
    m_deck.push_back(c10);
    m_deck.push_back(c11);
    m_deck.push_back(c12);

    shuffle();
    displayDeck();
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
    Card tmp=a;
    a=b;
    b=tmp;
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

void Deck::displayDeck() const{

    for(const Card &card : m_deck){
        std::cout << card.getName() << std::endl;
    }
        
    
}
