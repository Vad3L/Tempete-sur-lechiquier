#include <cstdlib>
#include <csignal>

#include <gf/Log.h>

#include "server/AnnexFonctionServer.h"
#include "common/Constants.h"
#include "common/Deck.h"
#include "common/Board.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    signal(SIGPIPE, SIG_IGN);
    while (true) {

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
                
                if(tsl::sendStartGame(client1, client2) == -1) {
                    tsl::sendEndGame(client1, client2, tsl::ChessStatus::SURRENDER); 
                    break;
                }
            
                tsl::Board board;
                tsl::ChessStatus gameStatus = tsl::ChessStatus::ON_GOING;
                bool player = true;
                bool promotion = false;
                tsl::GamePhase phase;
                phase.setCurrentPhase(tsl::Phase::AVANT_COUP);

                if(sendStartTurn(client1, tsl::ChessColor::WHITE) == -1){
                    tsl::sendEndGame(client1, client2, tsl::ChessStatus::SURRENDER); 
                    break;
                }
                
                // loop game
                while (true) {
                    board.m_moveAvailable.clear();
                    if (player) {
                        gf::Log::debug("------TOUR J1------\n");
                        if ((gameStatus = board.isGameOver(tsl::ChessColor::WHITE)) != tsl::ChessStatus::ON_GOING) {
                            break;
                        }
                        
                        int ret = tsl::performTurn(deck, phase, board, client1, client2, TwoHands.first, promotion);
                        if(ret == -1) {
                            break;
                        } else if (ret==0) {
                            player = false;
                            phase.setCurrentPhase(tsl::Phase::AVANT_COUP);
                            if(tsl::sendStartTurn(client2, tsl::ChessColor::BLACK) == -1){
                                break;
                            }
                            board.m_turnTo = tsl::ChessColor::BLACK;
                            if(debugPlateau) {
                                gf::Packet pack;
                                tsl::Debug d;
                                d.board = board.getFen();
                                pack.is(d);
                                tsl::sendingPacket(client2, pack);	
                            }
                        }
                    } else {
                        gf::Log::debug("------TOUR J2------\n");
                        if ((gameStatus = board.isGameOver(tsl::ChessColor::BLACK)) != tsl::ChessStatus::ON_GOING) {
                            break;
                        }

                        int ret = tsl::performTurn(deck, phase, board, client2, client1, TwoHands.second, promotion);
                        if(ret == -1) {
                            break;
                        } else if (ret == 0) {
                            player = true;
                            phase.setCurrentPhase(tsl::Phase::AVANT_COUP);
                            
                            if(tsl::sendStartTurn(client1, tsl::ChessColor::WHITE) == -1){
                                break;
                            }
                            board.m_turnTo = tsl::ChessColor::WHITE;
                            if(debugPlateau) {
                                gf::Packet pack;
                                tsl::Debug d;
                                d.board = board.getFen();
                                pack.is(d);
                                tsl::sendingPacket(client1, pack);	
                            }
                        
                        }
                    }
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
    }
    return EXIT_SUCCESS;
}