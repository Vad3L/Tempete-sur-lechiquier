#include <iostream>

#include <gf/Packet.h>
#include <gf/TcpListener.h>
#include <gf/TcpSocket.h>

#include "../protocole/protocole.h"

int main (int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
		return -1;
	}

    int port = atoi(argv[1]);
    gf::TcpListener listener(std::to_string(port));
    
    gf::TcpSocket client1 = listener.accept();

    if (client1) {
        gf::Packet packetC1;
        
        PartieRep rep1;
        rep1.err = OK;
        rep1.coulPion = ChessColor::WHITE;

        packetC1.is(rep1);
        if (gf::SocketStatus::Data != client1.sendPacket(packetC1)) {
            std::cerr << "erreur lors de l'envoie du packet au client 1";
        }

        gf::TcpSocket client2 = listener.accept();

        if (client2) {
            gf::Packet packetC2;

            PartieRep rep2;
            rep2.err = OK;
            rep2.coulPion = ChessColor::BLACK;

            packetC2.is(rep2);
            if (gf::SocketStatus::Data != client2.sendPacket(packetC2)) {
                std::cerr << "erreur lors de l'envoie du packet au client 2";
            }
        }

        client2.~TcpSocket();
    }
        
    client1.~TcpSocket();
        
    return 0;
}
