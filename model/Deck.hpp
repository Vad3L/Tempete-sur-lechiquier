#ifndef DECK_H
#define DECK_H

#include <vector>

#include "Card.hpp"

#define CARDS_DESCRIPTIONDIR "../model"

class Deck{
    public:
        Deck();

        void displayDeck();

        Card getFirst();
        int getNbCardInDeck();

        std::pair<std::vector<Card>,std::vector<Card>> distribute();

        std::vector<Card> m_deck;
        std::vector<Card> m_discard;

    private:
        void swap(Card &a, Card &b);
        void shuffle();
        
        void drop(Card m_card);


};

#endif // DECK_H
