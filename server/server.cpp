#include "AnnexFctServer.hpp"
#include <cstring>
class Carte{
    private:
        char name[20]; //Plus grand nom 15
        bool beforeTurn;
        bool afterTurn;
        bool replaceTurn;
        bool cancelMove;
        bool cancelCarte;

    public:
        Carte(char *name,bool before, bool after, bool replace, bool cancelMove, bool cancelCarte);
        //Getters
        char getTurn(); //return when we can use the card b for beforeTurn, a for after
        char* getEffect();//return return r for replace,cm for CancelMove and cc for CancelCarte
        char* getName(); // return the name of the card

        bool operator<(const Carte& rhs) const{
            

        }

};



Carte::Carte(char *name,bool before, bool after, bool replace, bool cancelMove, bool cancelCarte){
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

char Carte::getTurn(){
    if(this->beforeTurn){
        return 'b';
    }
    if(this->afterTurn){
        return 'a';
    }
    return 'd'; //for during
}

char* Carte::getEffect(){
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

char* Carte::getName(){
    return this->name;
}



int main (int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
		return -1;
	}

    int port = atoi(argv[1]);
    FctAnnex fctAnnex;

    gf::TcpListener listener(std::to_string(port));
    
    gf::TcpSocket client1 = listener.accept();

    if (client1) {
        gf::Packet packetC1;
        
        PartieRep rep1;
        rep1.err = NONE;
        rep1.coulPion = ChessColor::WHITE;

        packetC1.is(rep1);
        if (gf::SocketStatus::Data != client1.sendPacket(packetC1)) {
            std::cerr << "erreur lors de l'envoie du packet au client 1";
        }

        gf::TcpSocket client2 = listener.accept();

        if (client2) {
            gf::Packet packetC2;

            PartieRep rep2;
            rep2.err = NONE;
            rep2.coulPion = ChessColor::BLACK;

            packetC2.is(rep2);
            if (gf::SocketStatus::Data != client2.sendPacket(packetC2)) {
                std::cerr << "erreur lors de l'envoie du packet au client 2";
            }
            
            Plateau plateau;
            bool turnPlayer1 = true;

            //Création des cartes
            std::set<Carte> deck;
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
            deck.insert(c12);


            std::cout << "Je suis le serveur" << std::endl;
            while (true) {
                plateau.moveAvailable.clear();

                if (turnPlayer1) {

                    std::cout << "------TOUR J1------" << std::endl;
                    if(fctAnnex.performActionMoveNormal(&plateau, &client1, &client2, &packetC1, &packetC2, &turnPlayer1) == -1) {
                        return -1;
                    }
                    
                }else {

                    std::cout << "------TOUR J2------" << std::endl;
                    if(fctAnnex.performActionMoveNormal(&plateau, &client1, &client2, &packetC1, &packetC2, &turnPlayer1) == -1) {
                        return -1;
                    }

                }   
            }
        }

        client2.~TcpSocket();
    }
        
    client1.~TcpSocket();
        
    return 0;
}