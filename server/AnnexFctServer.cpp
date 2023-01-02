#include <iostream>

#include "AnnexFctServer.hpp"

FctAnnex::FctAnnex() {

}

// check validité d'un coup
bool FctAnnex::checkCoupValide(Plateau *plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
    if(coordStart.x < 0 || coordStart.y < 0 || coordEnd.x > 7 || coordEnd.y > 7) {
        return false;
    }
    
    Piece p = plateau->state[coordEnd.y * 8 + coordEnd.x].piece;
    if(p.getType() == ChessPiece::KING) {
        return false;
    }
    
    std::vector<gf::Vector2i> moveAvailable = plateau->state[coordStart.y * 8 + coordStart.x].piece.getMoves(gf::Vector2i(coordStart.x, coordStart.y));
    moveAvailable = plateau->filterMoveAuthorized(gf::Vector2i(coordStart.x, coordStart.y), moveAvailable);
    
    auto it =std::find(moveAvailable.begin(), moveAvailable.end(), gf::Vector2i(coordEnd.x, coordEnd.y));
    return it != moveAvailable.end(); 
}

CoupRep FctAnnex::buildRepCoup(Plateau *plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
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

int FctAnnex::performActionMoveNormal(Plateau *plateau, gf::TcpSocket *client1, gf::TcpSocket *client2, gf::Packet *packetP1, gf::Packet *packetP2, bool *turnPlayer1) {
    CoupReq coup;
    std::cout << "000000" << std::endl;
    if(*turnPlayer1) {
        if (gf::SocketStatus::Data != client1->recvPacket(*packetP1)) {
            std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 1";
            return -1;
        }
        std::cout << "11111" << std::endl;
        assert(packetP1->getType() == CoupReq::type);
        coup = packetP1->as<CoupReq>();
    }else {
        if (gf::SocketStatus::Data != client2->recvPacket(*packetP2)) {
            std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 2";
            return -1;
        }
        std::cout << "2222" << std::endl;
        assert(packetP2->getType() == CoupReq::type);
        coup = packetP2->as<CoupReq>();
    }
    
    std::cout << "33333333" << std::endl;
    CoupRep coupRep = buildRepCoup(plateau, gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
    std::cout << "44444" << std::endl;

    if(coupRep.err == CodeRep::NONE) { // coup valide
        std::cout << "------COUP VALIDE------" << std::endl;
        plateau->state[coup.posStart.y * 8 + coup.posStart.x].piece.isMoved = true;
        plateau->movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
        plateau->prettyPrint();
        *turnPlayer1 = !(*turnPlayer1);    
    }
    
    packetP1->is(coupRep);

    if (gf::SocketStatus::Data != client1->sendPacket(*packetP1)) {
        std::cerr<<"erreur lors de l'envoie du packet coupRep au client 1";
        return -1;
    }
    
    packetP2->is(coupRep);

    if (gf::SocketStatus::Data != client2->sendPacket(*packetP2)) {
        std::cerr<<"erreur lors de l'envoie du packet coupRep au client 2";
        return -1;
    }
    
    return 0;
}