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

#include "../model/Plateau.hpp"
#include "../protocole/Protocole.hpp"
#include "../model/GamePhase.hpp"

int performTurn (GamePhase& gp, Plateau& plateau, gf::TcpSocket& player, gf::TcpSocket& other, std::vector<Card>& hand, bool& promotion);
void performCard (Plateau& plateau, CardRep& c, std::vector<Card>& hand);
void performPromotion (Plateau& plateau, PromotionRep& p);
bool performCoup (Plateau& plateau, CoupRep& c);

int receivingPacket (gf::TcpSocket& client, gf::Packet &p);
int sendingPacket (gf::TcpSocket& client, gf::Packet &p);

void checkCardPacketValidity (Plateau& p, CardRep& r, std::vector<Card>& hand, Phase f);
void checkPromotionValidity (Plateau& plateau, PromotionRep& r);
void checkCoupPacketValidity (Plateau& plateau, CoupRep& c);
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd);
int sendStartOrEnd (gf::TcpSocket& a, gf::TcpSocket& b, CodeRep code, ChessStatus s = ChessStatus::ON_GOING, ChessColor c = ChessColor::NONE);
int sendStartTurn (gf::TcpSocket& a);
int sendInit(gf::TcpSocket& a, ChessColor c, std::vector<Card>& hand);

#endif // ANNEXFCTSERVER_H
