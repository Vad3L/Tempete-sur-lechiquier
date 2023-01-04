#include "Card.hpp"
#include <cstring>


Card::Card(char *name,bool before, bool after, bool replace, bool cancelMove, bool cancelCarte){
    strcpy(this->name,name);
    if(before && !after){
        this->beforeTurn=true;
    }
    if(after && !before){
        this->afterTurn=true;
    }
    if(replace && !after && !before){
        this->replaceTurn=true;
    }
    if(cancelMove && !cancelCarte){
        this->cancelMove=true;
    }
    if(cancelCarte && !cancelMove){
        this->cancelCarte=true;
    }
}

char Card::getTurn(){
    if(this->beforeTurn){
        return 'b';
    }
    if(this->afterTurn){
        return 'a';
    }
    return 'd'; //for during
}

char* Card::getEffect(){
    if(this->replaceTurn){
        return (char *)"r";
    }
    if(this->cancelCarte){
        return (char *)"cc";
    }
    if(this->cancelMove){
        return (char *)"cm";
    }

    return (char *)"n"; //for none
}

char* Card::getName(){
    return this->name;
}




 //Création des cartes
          /*  std::set<Carte> deck;
            Carte c1((char *)"Chameau",false,true,false,false,false);
            Carte c2((char *)"Coup d'etat",false,true,false,false,false);
            Carte c3((char *)"Pas question !",true,false,false,false,true);
            Carte c4((char *)"Niet.",true,false,false,false,true);
            Carte c5((char *)"Charge",false,false,true,false,false);
            Carte c6((char *)"Banzai",false,false,true,false,false);
            Carte c7((char *)"Amphetamines",false,true,false,false,false);
            Carte c8((char *)"Favorite",false,false,true,false,false);
            Carte c9((char *)"Bombe Atomique",false,true,false,false,false);
            Carte c10((char *)"Trappe",false,true,false,false,false);
            Carte c11((char *)"Quatre Coins",false,false,true,false,false);
            Carte c12((char *)"Exil",false,true,false,false,false);

            deck.insert(c1);
            deck.insert(c2);
            deck.insert(c3);
            deck.insert(c4);
            deck.insert(c5);
            deck.insert(c6);
            deck.insert(c7);
            deck.insert(c8);
            deck.insert(c9);
            deck.insert(c10);
            deck.insert(c11);
            deck.insert(c12);*/