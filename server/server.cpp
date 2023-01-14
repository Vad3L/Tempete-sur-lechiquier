#include "AnnexFctServer.hpp"
#include <csignal>
#include <cstring>

int main (int argc, char* argv[]) {
    int port = 43771;

    signal(SIGPIPE, SIG_IGN);
    gf::TcpListener listener(std::to_string(port));
    
    gf::TcpSocket client1 = listener.accept();

    if (client1) {
        gf::Packet packetC1;
        
        PartieRep rep1;
        rep1.err = COLOR;
        rep1.colorPion = ChessColor::WHITE;

        packetC1.is(rep1);
        if (gf::SocketStatus::Data != client1.sendPacket(packetC1)) {
            gf::Log::error("Lors de l'envoie du packet au client 1\n");
        }
        
        gf::TcpSocket client2 = listener.accept();
        if (client2) {
            gf::Packet packetC2;

            PartieRep rep2;
            rep2.err = COLOR;
            rep2.colorPion = ChessColor::BLACK;
            
            packetC2.is(rep2);
            if (gf::SocketStatus::Data != client2.sendPacket(packetC2)) {
                gf::Log::error("Lors de l'envoie du packet au client 2\n");
            }

	        if(sendStartOrEnd(client1, client2, CodeRep::GAME_START) == -1) {
            	sendStartOrEnd(client1, client2, CodeRep::GAME_END, ChessStatus::SURRENDER); 
                return -1;
            };
            
            Plateau plateau;
	    ChessStatus gameStatus = ChessStatus::ON_GOING;
            bool turnPlayer1 = true;
            bool promotion = false;
            while (true) {
                plateau.moveAvailable.clear();
                if (turnPlayer1) {
                    gf::Log::debug("------TOUR J1------\n");
                    if ((gameStatus = plateau.isGameOver(ChessColor::WHITE)) != ChessStatus::ON_GOING) {
                        break;
                    }

                    if(performAction(plateau, client1, client2, turnPlayer1, promotion) == -1) {
                        break;
                    }
                } else {
                    gf::Log::debug("------TOUR J2------\n");
                    if ((gameStatus = plateau.isGameOver(ChessColor::BLACK)) != ChessStatus::ON_GOING) {
                        break;
                    }

                    if(performAction(plateau, client1, client2, turnPlayer1, promotion) == -1) {
                        break;
                    }
                }
            }

            gf::Log::debug("------GAME END------\n");
            if (turnPlayer1 && gameStatus == ChessStatus::WIN) {
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::BLACK );
                gf::Log::debug("############noir gagne############\n");
            } else if (gameStatus == ChessStatus::WIN) {
                gf::Log::debug("############Blanc gagne############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::WHITE);
            } else if(gameStatus == ChessStatus::EQUALITY) {
                gf::Log::debug("############nulle############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus);
            }else {
                gf::Log::debug("############forfait############\n");
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, ChessStatus::SURRENDER); 
            }
        }

        client2.~TcpSocket();
    }
        
    client1.~TcpSocket();
        
    return 0;
}
