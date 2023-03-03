#ifndef TSL_ANNEXFONCTIONSERVER_H
#define TSL_ANNEXFONCTIONSERVER_H

#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/Vector.h>
#include <gf/Log.h>

#include "../common/Protocol.h"
#include "../common/GamePhase.h"

int receivingPacket (gf::TcpSocket& client, gf::Packet &p);
int sendingPacket (gf::TcpSocket& client, gf::Packet &p);

int sendStartGame (gf::TcpSocket& a, gf::TcpSocket& b);
int sendEndGame (gf::TcpSocket& a, gf::TcpSocket& b, tsl::ChessStatus chessStatus, tsl::ChessColor chessColor = tsl::ChessColor::NONE);
int sendStartTurn (gf::TcpSocket& a, tsl::ChessColor turnTo);
int sendColorDeckCards(gf::TcpSocket& client, tsl::ChessColor c, std::array<int, 5> hand);

#endif // TSL_ANNEXFONCTIONSERVER_H