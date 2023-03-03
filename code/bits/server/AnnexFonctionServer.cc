#include "AnnexFonctionServer.h"


int receivingPacket (gf::TcpSocket& s, gf::Packet &p) {
    if (!s) {
        gf::Log::error("Client déconnecté\n");
        return -1;
    }

    if (gf::SocketStatus::Data != s.recvPacket(p)) {
        gf::Log::error("Lors de la réception du packet qui contient le coup du client\n");
        return -1;
    }

    return 0;
}

int sendingPacket (gf::TcpSocket& s, gf::Packet& p) {
    if (!s) {
        gf::Log::error("Client déconnecté");
        return -1;
    }

    if (gf::SocketStatus::Data != s.sendPacket(p)) {

        gf::Log::error("Lors de l'envoie du packet coupRep au client\n");
        return -1;
    }

    return 0;
}

int sendStartGame (gf::TcpSocket& a, gf::TcpSocket& b) {
    int ret = 0;
    gf::Packet packet;

    tsl::StartGame start;
    packet.is(start);
    
    if (gf::SocketStatus::Data != a.sendPacket(packet)) {
        gf::Log::error("Lors de l'envoie du packet de debut ou fin au client 1\n");
        ret = -1;
    }
    
    if (gf::SocketStatus::Data != b.sendPacket(packet)) {
        gf::Log::error("Lors de l'envoi du packet de debut fin au client 2\n");
        ret = -1;
    }

    return ret;
}

int sendEndGame (gf::TcpSocket& a, gf::TcpSocket& b, tsl::ChessStatus chessStatus, tsl::ChessColor chessColor) {
    int ret = 0;
    gf::Packet packet;

    tsl::EndGame end;
    end.chessStatus = chessStatus;
    end.chessColor = chessColor;
    packet.is(end);
    
    if (gf::SocketStatus::Data != a.sendPacket(packet)) {
        gf::Log::error("Lors de l'envoie du packet de debut ou fin au client 1\n");
        ret = -1;
    }
    
    if (gf::SocketStatus::Data != b.sendPacket(packet)) {
        gf::Log::error("Lors de l'envoi du packet de debut fin au client 2\n");
        ret = -1;
    }

    return ret;
}

int sendStartTurn (gf::TcpSocket& a, tsl::ChessColor turnTo){
    int ret = 0;
    gf::Packet packet;

    tsl::StartTurn start;
    start.colorPlayer = turnTo;
    packet.is(start);

    if (gf::SocketStatus::Data != a.sendPacket(packet)) {
        gf::Log::error("Lors de l'envoie du packet de debut de tour\n");
        ret = -1;
    }

    return ret;
}

int sendColorDeckCards(gf::TcpSocket& client, tsl::ChessColor c, std::array<int, 5> hand) {
	gf::Packet pack;
		
    tsl::ColorDeck colorDeck;
    colorDeck.colorPlayer = c;
    colorDeck.hand = hand;
    pack.is(colorDeck);

    if (gf::SocketStatus::Data != client.sendPacket(pack)) {
        gf::Log::error("Lors de l'envoie du packet contenant la couleur au client\n");
        return -1;
    }

	return 0;
}

