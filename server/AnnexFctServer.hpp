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

#include "../model/chess.hpp"
#include "../model/Plateau.hpp"
#include "../protocole/protocole.h"

class FctAnnex {
	public:
		FctAnnex();

		bool checkCoupValide(Plateau *plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd);
		CoupRep buildRepCoup(Plateau *plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd);
		int performActionMoveNormal(Plateau *plateau, gf::TcpSocket *client1, gf::TcpSocket *client2, gf::Packet *packetP1, gf::Packet *packetP2, bool *turnPlayer1);
};


#endif // ANNEXFCTSERVER_H