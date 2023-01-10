#include "AnnexFctServer.hpp"

// check validité d'un coup
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
    if (coordStart.x < 0 || coordStart.y < 0 || coordEnd.x > 7 || coordEnd.y > 7) {
        return false;
    }
    
    Piece pieceStart = plateau.state[coordStart.y * 8 + coordStart.x].piece;
    Piece p = plateau.state[coordEnd.y * 8 + coordEnd.x].piece;

    ChessColor last = ChessColor::BLACK;
    if(plateau.lastCoup.size() > 0) {
        last = plateau.state[plateau.lastCoup.back().y * 8 + plateau.lastCoup.back().x].piece.getColor();
    }
    
    if(pieceStart.getColor() == ChessColor::NONE || pieceStart.getColor() == last) {
        return false;
    }

    if(p.getType() == ChessPiece::KING) {
        return false;
    }
    
    std::vector<gf::Vector2i> moveAvailable = plateau.state[coordStart.y * 8 + coordStart.x].piece.getMoves(gf::Vector2i(coordStart.x, coordStart.y));
    moveAvailable = plateau.filterMoveAuthorized(gf::Vector2i(coordStart.x, coordStart.y), moveAvailable);
    
    auto it = std::find(moveAvailable.begin(), moveAvailable.end(), gf::Vector2i(coordEnd.x, coordEnd.y));
    return it != moveAvailable.end(); 
}

gf::Packet recvPacket(gf::TcpSocket& s) {
	gf::Packet p;
	if (!s) {
		std::cerr << "client déconnecté";
		exit(1);
	}

	if (gf::SocketStatus::Data != s.recvPacket(p)) {
        	std::cerr << "erreur lors de la réception du packet qui contient le coup du client";
                exit(1);
	}

	return p;
}

void checkCoupPacketValidity (Plateau& plateau, CoupRep& r) {
    assert(r.posStart.x >= 0);
    assert(r.posStart.x < 8);
    assert(r.posStart.y >= 0);
    assert(r.posStart.y < 8);

    assert(r.posEnd.x >= 0);
    assert(r.posEnd.x < 8);
    assert(r.posEnd.y >= 0);
    assert(r.posEnd.y < 8);

    bool coupValide = checkCoupValide(plateau, r.posStart, r.posEnd);

    if (coupValide) {   
        r.err = CodeRep::NONE;
    } else {
        r.err = CodeRep::COUP_NO_VALIDE;
    }
}

void checkPromotionValidity (Plateau& plateau, PromotionRep& r) {
    assert(r.pos.x >= 0);
    assert(r.pos.x < 8);
    assert(r.pos.y >= 0);
    assert(r.pos.y < 8);

    bool promoValide = false;
    Piece piece = plateau.state[r.pos.y * 8 + r.pos.x].piece;
    Case c = plateau.state[r.pos.y * 8 + r.pos.x];    

    if (piece.getType() != ChessPiece::PAWN) {
	promoValide = false;
    }

    if (c.position.y == 7 && piece.getColor() == ChessColor::WHITE) {
	promoValide &= true;
    }

    if (c.position.y == 0 && piece.getColor() == ChessColor::BLACK) {
	promoValide &= true;
    }

    if (!promoValide) {
	exit(1); // coup invalide
    }
}

int performActionMoveNormal(Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1, bool &promotion) {
    CoupRep coup;
    PromotionRep promo;
    gf::Packet packet;
    gf::Packet packet2;

    if (!promotion) {
        if (turnPlayer1) {
            if (gf::SocketStatus::Data != client1.recvPacket(packet)) {
                std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 1";
                return -1;
            }
            
            assert(packet.getType() == CoupRep::type);
            coup = packet.as<CoupRep>();
        } else {
            if (gf::SocketStatus::Data != client2.recvPacket(packet)) {
                std::cerr<<"erreur lors de la réception du packet qui contient le coup du client 2";
                return -1;
            }
            
            assert(packet.getType() == CoupRep::type);
            coup = packet.as<CoupRep>();
        }

        checkCoupPacketValidity(plateau, coup);

        if (coup.err == CodeRep::NONE) { // coup valide
            std::cout << "------COUP VALIDE------" << std::endl;
            Piece p = plateau.state[coup.posStart.y * 8 + coup.posStart.x].piece;
            p.isMoved = true;
            plateau.movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
            plateau.prettyPrint();

            plateau.lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
            plateau.lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   

            if(p.getType() == ChessPiece::PAWN && (coup.posEnd.y == 0 || coup.posEnd.y ==7)) {
                promotion = true;
            } else {
                plateau.allPositions.push_back(plateau.getFen());
                std::cout << "position : " << plateau.allPositions.back() << std::endl;
                turnPlayer1 = !(turnPlayer1);
            }

            plateau.coordPrisePassant = gf::Vector2i(-1); 
        } else {
            return 2;
        }
        
        packet.is(coup);

        if (gf::SocketStatus::Data != client1.sendPacket(packet)) {
            std::cerr<<"erreur lors de l'envoie du packet coupRep au client 1";
            exit(-1);
        }

        packet.is(coup);

        if (gf::SocketStatus::Data != client2.sendPacket(packet)) {
            std::cerr<<"erreur lors de l'envoie du packet coupRep au client 2";
            exit(-1);
        }
    }

    if (promotion) {
        if(turnPlayer1) {
            if (gf::SocketStatus::Data != client1.recvPacket(packet2)) {
                std::cerr<<"erreur lors de la réception du packet qui contient la promotion du client 1";
                return -1;
            }
        
            assert(packet2.getType() == PromotionRep::type);
            promo = packet2.as<PromotionRep>();
        }else {
            if (gf::SocketStatus::Data != client2.recvPacket(packet2)) {
                std::cerr<<"erreur lors de la réception du packet qui contient la promotion du client 2";
                return -1;
            }
            
            assert(packet2.getType() == PromotionRep::type);
            promo = packet2.as<PromotionRep>();  
        }

	checkPromotionValidity(plateau, promo); 

        if(promo.err == CodeRep::NONE) { // coup valide
            std::cout << "------PROMO VALIDE------" << std::endl;
            
            plateau.promotionPiece(gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice);
            promotion = false;
            plateau.allPositions.push_back(plateau.getFen());
            std::cout << "position : " << plateau.allPositions.back() << std::endl;
            turnPlayer1 = !(turnPlayer1);
        } else {
            return 2;
        }

        packet2.is(promo);

        if (gf::SocketStatus::Data != client1.sendPacket(packet2)) {
            std::cerr<<"erreur lors de l'envoie du packet coupRep au client 1";
            return -1;
        }
        
        packet2.is(promo);

        if (gf::SocketStatus::Data != client2.sendPacket(packet2)) {
            std::cerr<<"erreur lors de l'envoie du packet coupRep au client 2";
            return -1;
        }
      
    }

    return 0;
}

int sendStartOrEnd (gf::TcpSocket& a, gf::TcpSocket& b, CodeRep code, ChessStatus s, ChessColor c) {
	gf::Packet packet;
	PartieRep rep;
	rep.err = code;
	rep.coulPion = c;
	rep.status = s;
	packet.is(rep);
    
	if (gf::SocketStatus::Data != a.sendPacket(packet)) {
		std::cerr << "erreur lors de l'envoie du packet au client 1";
        	return -1;
	}

	if (gf::SocketStatus::Data != b.sendPacket(packet)) {
		std::cerr << "erreur lors de l'envoi du packet au client 2";
        	return -1;
	}

    	return 0;
}

