#include <iostream>
#include <string>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>

#include "../protocole/protocole.h"

int status = 1;

int main (int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: ./server [port]" << std::endl;
		return 0;
	}

	int port = atoi(argv[1]);
	gf::TcpListener listener(std::to_string(port));
	gf::TcpSocket client = listener.accept();

	if (client) {
		gf::Packet ordre;
		gf::Packet paquet;
		while (true) {
			if (gf::SocketStatus::Data != client.recvPacket(paquet)) {
				std::cerr << "Erreur reception paquet" << std::endl;
				return -2;		
			}

			auto req = paquet.as<Paquet>();
			if (req.choix != 1 && req.choix != 2) {
				std::cerr << "Erreur de protocole" << std::endl;
				return -3;
			}

			status = req.choix;
			std::cout << "le nouveau status est " << req.choix << std::endl;

			Paquet p;
			p.choix = status;
			ordre.is(p);
			if (gf::SocketStatus::Data != client.sendPacket(ordre)) {
				std::cerr << "Erreur lors de l'envoi du paquet" << std::endl;
				return -1;
			}
		}
	}

	client.~TcpSocket();
	return 0;
	
}
