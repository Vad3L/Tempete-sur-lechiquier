#ifndef ANNEXFCTSERVER_H
#define ANNEXFCTSERVER_H

#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>
#include <gf/Vector.h>
#include <gf/VectorOps.h>
#include <gf/Log.h>

#include "../model/chess.hpp"
#include "../model/Plateau.hpp"
#include "../protocole/protocole.h"

int performActionMoveNormal(Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1, bool &promotion);

// REWORKED

gf::Packet recvPacket (gf::TcpSocket& client);
void checkPromotionValidity (Plateau& plateau, PromotionRep& r);
void checkCoupPacketValidity (Plateau& plateau, CoupRep& c);
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd);
int sendStartOrEnd (gf::TcpSocket& a, gf::TcpSocket& b, CodeRep code, ChessStatus s = ChessStatus::ON_GOING, ChessColor c = ChessColor::NONE);

#endif // ANNEXFCTSERVER_H
