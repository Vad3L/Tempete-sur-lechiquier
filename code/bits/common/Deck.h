#ifndef TSL_DECK_H
#define TSL_DECK_H

#include <vector>
#include <fstream>

#include "Card.h"

#define CARDS_DESCRIPTIONDIR "../data/"

namespace tsl {

    class Deck{
        public:
            Deck(std::vector<int> numCards = {});

            void displayDeck();

            Card getFirst();
            std::size_t getNbCardInDeck();
            void emptyDiscard ();
            void drop (Card c);
            std::pair<std::array<int, 5>,std::array<int, 5>> distribute();
            std::vector<Card> getDeck() const;

        private:
            void swap(Card &a, Card &b);
            void shuffle();

            std::vector<Card> m_deck;
            std::vector<Card> m_discard;
    };

}

#endif // TSL_DECK_H