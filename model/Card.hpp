#ifndef CARD_H
#define CARD_H


class Card{
    private:
        char name[20]; //Plus grand nom 15
        bool beforeTurn;
        bool afterTurn;
        bool replaceTurn;
        bool cancelMove;
        bool cancelCarte;

    public:
        Card(char *name,bool before, bool after, bool replace, bool cancelMove, bool cancelCarte);
        //Getters
        char getTurn(); //return when we can use the card b for beforeTurn, a for after
        char* getEffect();//return return r for replace,cm for CancelMove and cc for CancelCarte
        char* getName(); // return the name of the card

		bool operator<(const Card& other) const ;

};

#endif // CARD_H
