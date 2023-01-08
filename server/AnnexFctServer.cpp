#include "AnnexFctServer.hpp"

// check validité d'un coup
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
    if(coordStart.x < 0 || coordStart.y < 0 || coordEnd.x > 7 || coordEnd.y > 7) {
        return false;
    }
    
    Piece p = plateau.state[coordEnd.y * 8 + coordEnd.x].piece;
    if(p.getType() == ChessPiece::KING) {
        return false;
    }
    
    std::vector<gf::Vector2i> moveAvailable = plateau.state[coordStart.y * 8 + coordStart.x].piece.getMoves(gf::Vector2i(coordStart.x, coordStart.y));
    moveAvailable = plateau.filterMoveAuthorized(gf::Vector2i(coordStart.x, coordStart.y), moveAvailable);
    
    auto it = std::find(moveAvailable.begin(), moveAvailable.end(), gf::Vector2i(coordEnd.x, coordEnd.y));
    return it != moveAvailable.end(); 
}

CoupRep buildRepCoup(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
    assert(coordStart.x >= 0);
    assert(coordStart.x < 8);
    assert(coordStart.y >= 0);
    assert(coordStart.y < 8);

    bool coupValide = checkCoupValide(plateau, coordStart, coordEnd);
    CoupRep coupRep;

    coupRep.posStart.x = coordStart.x;
    coupRep.posStart.y = coordStart.y;
    coupRep.posEnd.x = coordEnd.x;
    coupRep.posEnd.y = coordEnd.y;

    if(coupValide) { // coup valide				    
        coupRep.err = CodeRep::NONE;
    }else {
        coupRep.err = CodeRep::COUP_NO_VALIDE;
    }
    
    return coupRep;
}

int performActionMoveNormal(Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1) {
    CoupReq coup;
    gf::Packet packet;

    if(turnPlayer1) {
        if (gf::SocketStatus::Data != client1.recvPacket(packet)) {
            std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 1";
            return -1;
        }
        
        assert(packet.getType() == CoupReq::type);
        coup = packet.as<CoupReq>();
    }else {
        if (gf::SocketStatus::Data != client2.recvPacket(packet)) {
            std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 2";
            return -1;
        }
        
        assert(packet.getType() == CoupReq::type);
        coup = packet.as<CoupReq>();
    }

    CoupRep coupRep = buildRepCoup(plateau, gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));

    if(coupRep.err == CodeRep::NONE) { // coup valide
        std::cout << "------COUP VALIDE------" << std::endl;
        plateau.state[coup.posStart.y * 8 + coup.posStart.x].piece.isMoved = true;
        plateau.movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
        plateau.prettyPrint();

        plateau.lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
        plateau.lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   
	plateau.all_position.push_back(plateau.getFen());
	std::cout << "position : " << plateau.all_position.back() << std::endl;
 
        turnPlayer1 = !(turnPlayer1);
        plateau.prisePassant = false; 
    }
    
    packet.is(coupRep);

    if (gf::SocketStatus::Data != client1.sendPacket(packet)) {
        std::cerr<<"erreur lors de l'envoie du packet coupRep au client 1";
        return -1;
    }
    
    packet.is(coupRep);

    if (gf::SocketStatus::Data != client2.sendPacket(packet)) {
        std::cerr<<"erreur lors de l'envoie du packet coupRep au client 2";
        return -1;
    }
    
    return 0;
}

void sendStart (gf::TcpSocket& a, gf::TcpSocket& b) {
	gf::Packet packet;

	PartieRep rep;
	rep.err = GAME_START;
	rep.coulPion = ChessColor::BLACK;

	packet.is(rep);

	if (gf::SocketStatus::Data != a.sendPacket(packet)) {
		std::cerr << "erreur lors de l'envoie du packet au client 1";
	}

	if (gf::SocketStatus::Data != b.sendPacket(packet)) {
		std::cerr << "erreur lors de l'envoi du packet au client 2";
	}
}


