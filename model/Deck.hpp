#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.hpp"

class Deck{
    public:
        Deck();
        void displayDeck() const;
        Card getFirst();
        int getNbCardInDeck();

        std::pair<std::vector<Card>,std::vector<Card>> distribute();
    private:
        std::vector<Card> m_deck;
        std::vector<Card> m_discard;

    private:
        void swap(Card &a, Card &b);
        void shuffle();
        
        void drop(Card m_card);


};

#endif // DECK_H
