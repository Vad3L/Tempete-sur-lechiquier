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

#include "../model/Chess.hpp"
#include "../model/Plateau.hpp"
#include "../protocole/Protocole.hpp"

void performPromotion (Plateau& plateau, PromotionRep& p);
bool performCoup (Plateau& plateau, CoupRep& c);
int performAction (Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1, bool &promotion);

int recvPacket (gf::TcpSocket& client, gf::Packet &p);
int sendPacket (gf::TcpSocket& client);

void checkPromotionValidity (Plateau& plateau, PromotionRep& r);
void checkCoupPacketValidity (Plateau& plateau, CoupRep& c);
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd);
int sendStartOrEnd (gf::TcpSocket& a, gf::TcpSocket& b, CodeRep code, ChessStatus s = ChessStatus::ON_GOING, ChessColor c = ChessColor::NONE);
void sendStartTurn (gf::TcpSocket& a);
#endif // ANNEXFCTSERVER_H
