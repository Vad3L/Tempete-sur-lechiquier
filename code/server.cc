#include <cstdlib>
#include <csignal>


#include "server/AnnexFonctionServer.h"
#include "common/Constants.h"
#include "common/Deck.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    signal(SIGPIPE, SIG_IGN);

    gf::TcpListener listener(tsl::Service);
    
    std::vector<int> nameCards;
    bool debugPlateau = false;
    if (argc >= 2 && std::string(argv[1]) == "--debug") {
        gf::Log::debug("(SERVEUR) Debug mode\n");
        for (int i=2; i<argc; i++) {
            nameCards.push_back(atoi(argv[i]));
        }
        debugPlateau = true;
    }

    tsl::Deck deck(nameCards);
    auto TwoHands = deck.distribute();
       
    gf::TcpSocket client1 = listener.accept();
    
    if (client1) {
        
        gf::TcpSocket client2 = listener.accept();
        if (client2) {
        
            sendColorDeckCards(client1, tsl::ChessColor::WHITE, TwoHands.first);
            sendColorDeckCards(client2, tsl::ChessColor::BLACK, TwoHands.second);
            
            if(sendStartGame(client1, client2) == -1) {
                sendEndGame(client1, client2, tsl::ChessStatus::SURRENDER); 
            }
        
            //Plateau plateau;
            tsl::ChessStatus gameStatus = tsl::ChessStatus::ON_GOING;
            bool player = true;
            bool promotion = false;
            tsl::GamePhase phase;

            if(sendStartTurn(client1, tsl::ChessColor::WHITE) == -1){
            }
            
            // loop game
            while (true) {
               
            }

            gf::Log::debug("------GAME END------\n");
            if (player && gameStatus == tsl::ChessStatus::WIN) {
                sendEndGame(client1, client2, gameStatus, tsl::ChessColor::BLACK );
                gf::Log::debug("############noir gagne############\n");
            } else if (gameStatus == tsl::ChessStatus::WIN) {
                gf::Log::debug("############Blanc gagne############\n");
                sendEndGame(client1, client2, gameStatus, tsl::ChessColor::WHITE);
            } else if(gameStatus == tsl::ChessStatus::EQUALITY) {
                gf::Log::debug("############nulle############\n");
                sendEndGame(client1, client2, gameStatus);
            } else {
                gf::Log::debug("############forfait############\n");
                sendEndGame(client1, client2, tsl::ChessStatus::SURRENDER); 
            }
        }
        client2.~TcpSocket();
    }	
    client1.~TcpSocket();

    return EXIT_SUCCESS;
}