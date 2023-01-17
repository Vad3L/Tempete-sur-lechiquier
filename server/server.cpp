#include "AnnexFctServer.hpp"
#include "../model/Deck.hpp"

#include <csignal>

int main (int argc, char* argv[]) {
    int port = 43771;

    signal(SIGPIPE, SIG_IGN);
    gf::TcpListener listener(std::to_string(port));
    
    gf::TcpSocket client1 = listener.accept();
    Deck deck;
    auto TwoHand = deck.distribute();
    
    if (client1) {
	    sendInit(client1, ChessColor::WHITE, TwoHand.first);
        
        gf::TcpSocket client2 = listener.accept();
        if (client2) {
	    sendInit(client2, ChessColor::BLACK, TwoHand.second);
	    
	    if(sendStartOrEnd(client1, client2, CodeRep::GAME_START) == -1) {
            sendStartOrEnd(client1, client2, CodeRep::GAME_END, ChessStatus::SURRENDER); 
            return -1;
        }
        
        Plateau plateau;
	    ChessStatus gameStatus = ChessStatus::ON_GOING;
            bool player = true;
            bool promotion = false;
            while (true) {
                plateau.moveAvailable.clear();
                if (player) {
                    gf::Log::debug("------TOUR J1------\n");
                    if ((gameStatus = plateau.isGameOver(ChessColor::WHITE)) != ChessStatus::ON_GOING) {
                        break;
                    }
                    if(sendStartTurn(client1) == -1){
                        break;
                    }
		             int ret = performAction(plateau, client1, client2, promotion);
                    if(ret == -1) {
                        break;
                    } else if (!promotion && ret==0) { player = false; }
                } else {
                    gf::Log::debug("------TOUR J2------\n");
                    if ((gameStatus = plateau.isGameOver(ChessColor::BLACK)) != ChessStatus::ON_GOING) {
                        break;
                    }
                    if(sendStartTurn(client2) == -1){
                        break;
                    }
		            int ret = performAction(plateau, client2, client1, promotion);
                    if(ret == -1) {
                        break;
                    } else if (!promotion && ret == 0) { player = true; }
                }
            }

            gf::Log::debug("------GAME END------\n");
            if (player && gameStatus == ChessStatus::WIN) {
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::BLACK );
                gf::Log::debug("############noir gagne############\n");
            } else if (gameStatus == ChessStatus::WIN) {
                gf::Log::debug("############Blanc gagne############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::WHITE);
            } else if(gameStatus == ChessStatus::EQUALITY) {
                gf::Log::debug("############nulle############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus);
            } else {
                gf::Log::debug("############forfait############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, ChessStatus::SURRENDER); 
            }
        }

        client2.~TcpSocket();
    }
        
    client1.~TcpSocket();
        
    return 0;
}
