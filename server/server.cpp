#include "AnnexFctServer.hpp"
#include "../model/Deck.hpp"	

#include <csignal>

int main (int argc, char* argv[]) {
	int port = 43771;

	signal(SIGPIPE, SIG_IGN);
	gf::TcpListener listener(std::to_string(port));
	
	std::vector<int> numCards;

	bool debugPlateau = false;
	if (argc >= 2 && std::string(argv[1]) == "--debug") {
		gf::Log::debug("(SERVEUR) est lancé en mode debug\n");
		for (int i=2; i<argc; i++) {
			numCards.push_back(atoi(argv[i]));
		}
		debugPlateau = true;
	}

	gf::TcpSocket client1 = listener.accept();
	
	Deck deck(numCards);
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
		GamePhase phase;
		while (true) {
				plateau.moveAvailable.clear();
				if (player) {
					gf::Log::debug("------TOUR J1------\n");
					if ((gameStatus = plateau.isGameOver(ChessColor::WHITE)) != ChessStatus::ON_GOING) {
						break;
					}
					if (phase.getCurrentPhase() == Phase::AVANT_COUP) {
						if(sendStartTurn(client1) == -1){
							break;
						}
						plateau.turnTo = ChessColor::WHITE;
						if(debugPlateau) {
							gf::Packet pack;
							Debug d;
							d.plateau = plateau.getFen();
							pack.is(d);
							sendingPacket(client1, pack);	
						}
					}

					int ret = performTurn(deck, phase, plateau, client1, client2, TwoHand.first, promotion);

					if(ret == -1) {
						break;
					} else if (ret==0) {
						player = false;
						phase.setCurrentPhase(Phase::AVANT_COUP);
					}
				} else {
					gf::Log::debug("------TOUR J2------\n");
					if ((gameStatus = plateau.isGameOver(ChessColor::BLACK)) != ChessStatus::ON_GOING) {
						break;
					}
					if (phase.getCurrentPhase() == Phase::AVANT_COUP) {
						if(sendStartTurn(client2) == -1){
							break;
						}
						plateau.turnTo = ChessColor::BLACK;
						if(debugPlateau) {
							gf::Packet pack;
							Debug d;
							d.plateau = plateau.getFen();
							pack.is(d);
							sendingPacket(client2, pack);	
						}
					}

					int ret = performTurn(deck, phase, plateau, client2, client1, TwoHand.second, promotion);
					if(ret == -1) {
						break;
					} else if (ret == 0) {
						player = true;
						phase.setCurrentPhase(Phase::AVANT_COUP);
					}
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
