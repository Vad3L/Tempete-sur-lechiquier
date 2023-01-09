#include "AnnexFctServer.hpp"
#include <cstring>

int main (int argc, char* argv[]) {

    int port = 43771;
    //FctAnnex fctAnnex;

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

	        if(sendStartOrEnd(client1, client2, CodeRep::GAME_START) == -1) {
                return -1;
            };
            

            Plateau plateau;
	        ChessStatus gameStatus = ChessStatus::ON_GOING;
            bool turnPlayer1 = true;
            std::cout << "Je suis le serveur" << std::endl;
            while (true) {
                plateau.moveAvailable.clear();

                if (turnPlayer1) {
                    std::cout << "------TOUR J1------" << std::endl;
		            if ((gameStatus = plateau.isGameOver(ChessColor::WHITE)) != ChessStatus::ON_GOING) {
			            break;
		            }

                    if(performActionMoveNormal(plateau, client1, client2, turnPlayer1) == -1) {
                        break;
                    }
                } else {
                    std::cout << "------TOUR J2------" << std::endl;
		            if ((gameStatus = plateau.isGameOver(ChessColor::BLACK)) != ChessStatus::ON_GOING) {
			            break;
		            }

                    if(performActionMoveNormal(plateau, client1, client2, turnPlayer1) == -1) {
                        break;;
                    }
                }
            }

            if (turnPlayer1 && gameStatus == ChessStatus::WIN) {
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::BLACK );
                std::cout << "############Black win############\n";
            } else if (gameStatus == ChessStatus::WIN) {
                std::cout << "############white win############\n";
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus, ChessColor::WHITE);
            }else {
                std::cout << "############nulle############\n";
                sendStartOrEnd(client1, client2, CodeRep::GAME_END, gameStatus);
            }
        }

        client2.~TcpSocket();
    }
        
    client1.~TcpSocket();
        
    return 0;
}
