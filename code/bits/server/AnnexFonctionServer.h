#ifndef TSL_ANNEXFONCTIONSERVER_H
#define TSL_ANNEXFONCTIONSERVER_H

#include <array>
#include <cassert>
#include <iostream>
#include <algorithm>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/Vector.h>

#include "../common/Protocol.h"
#include "../common/GamePhase.h"
#include "../common/Board.h"
#include "../common/Deck.h"
#include "../common/Protocol.h"

namespace tsl {
    
    int performTurn (Deck& d, GamePhase& gp, Board& board, gf::TcpSocket& player, gf::TcpSocket& other, std::array<Card, 5>& hand, bool& promotion);
    bool performCard (Board& board, CardRep& c, std::array<Card, 5>& hand, Deck& d);
    void performPromotion (Board& board, PromotionRep& p);
    bool performCoup (Board& board, CoupRep& c);

    int receivingPacket (gf::TcpSocket& client, gf::Packet &p);
    int sendingPacket (gf::TcpSocket& client, gf::Packet &p);

    void checkCardPacketValidity (Board& board, CardRep& r, std::array<Card, 5>& hand, Phase f);
    void checkPromotionValidity (Board& board, PromotionRep& r);
    void checkCoupPacketValidity (Board& board, CoupRep& c);
    bool checkValidityCoup(Board& board, gf::Vector2i coordStart, gf::Vector2i coordEnd);

    int sendStartGame (gf::TcpSocket& a, gf::TcpSocket& b);
    int sendEndGame (gf::TcpSocket& a, gf::TcpSocket& b, ChessStatus chessStatus, ChessColor chessColor = ChessColor::NONE);
    int sendStartTurn (gf::TcpSocket& a, ChessColor turnTo);
    int sendColorDeckCards(gf::TcpSocket& client, ChessColor c, std::array<Card, 5> hand);
    void sendCard (gf::TcpSocket& player, Card new_card);
} 
#endif // TSL_ANNEXFONCTIONSERVER_H