#ifndef DECK_H
#define DECK_H

#include <vector>

#include "Card.hpp"




class Deck{
    public:
        Deck();
        void displayDeck() const;
    private:
        std::vector<Card> m_deck;
        std::vector<Card> m_discard;

    private:
        void swap(Card &a, Card &b);
        void shuffle();
        Card getFirst();
        void drop(Card m_card);


};

#endif // DECK_H
