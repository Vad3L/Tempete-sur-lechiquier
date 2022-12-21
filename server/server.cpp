#include <iostream>
#include <string>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>

#include "serverTools.hpp"

int status = 1;

int main (int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
		return 0;
	}

	int port = atoi(argv[1]);
	gf::TcpListener listener(std::to_string(port));
	gf::TcpSocket client1 = listener.accept();
	
	if (client1) {
		gf::TcpSocket client2 = listener.accept();

		if (client2) {
			gf::Packet packetC1;
			gf::Packet packetC2;

			if (gf::SocketStatus::Data != client1.recvPacket(packetC1)) {
				std::cerr << "erreur lors de la réception du packet du client 1";
				return -1;
			}

			auto req1 = packetC1.as<TPartieReq>();

			if (gf::SocketStatus::Data != client2.recvPacket(packetC2)) {
				std::cerr << "erreur lors de la réception du packet du client 2";
				return -1;
			}

			auto req2 = packetC2.as<TPartieReq>();

			std::vector<PartieRep> partieRep;
			partieRep = buildRepPartie(req1, req2);

			packetC1.is(partieRep.at(0));
			if (gf::SocketStatus::Data != client1.sendPacket(packetC1)) {
				std::cerr << "erreur lors de la réception du packet du client 1";
			}

			packetC2.is(partieRep.at(1));
			if (gf::SocketStatus::Data != client2.sendPacket(packetC2)) {
				std::cerr << "erreur lors de la réception du packet du client 2";
			}

			client2.~TcpSocket();
		}

		client1.~TcpSocket();
	}

    return 0;
}