#include "AnnexFctServer.hpp"

// check validité d'un coup
bool checkCoupValide(Plateau& plateau, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
    if(coordStart.x < 0 || coordStart.y < 0 || coordEnd.x > 7 || coordEnd.y > 7) {
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

PromotionRep buildRepPromotion(Plateau& plateau, gf::Vector2i coord, ChessPiece c) {
    assert(coord.x >= 0);
    assert(coord.x < 8);
    assert(coord.y >= 0);
    assert(coord.y < 8);

    bool promoValide = true;
    Piece piece = plateau.state[coord.y * 8 + coord.x].piece;
    
    //TO DO VERIFIE PROMO VALIDE
    /*for(auto &c : plateau.state) {
        if(c.piece.getType() != ChessPiece::PAWN || coord != c.position) {
            continue;
        } 
        if(c.piece.getColor() == ChessColor::WHITE && c.position.y == 7) {
            promoValide = true;
        }
        if(c.piece.getColor() == ChessColor::BLACK && c.position.y == 0) {
            promoValide = true;
        }
    }*/

    PromotionRep promoRep;

    promoRep.pos.x = coord.x;
    promoRep.pos.y = coord.y;
    promoRep.choice = c;

    if(promoValide) { // promotion valide				    
        promoRep.err = CodeRep::NONE;
    }else {
        promoRep.err = CodeRep::COUP_NO_VALIDE;
    }
    
    return promoRep;
}

int performActionMoveNormal(Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1, bool &promotion) {
    CoupReq coup;
    PromotionReq promo;
    gf::Packet packet;
    gf::Packet packet2;

    if(!promotion) {
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
            Piece p = plateau.state[coup.posStart.y * 8 + coup.posStart.x].piece;
            p.isMoved = true;
            plateau.movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
            plateau.prettyPrint();

            plateau.lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
            plateau.lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   

            if(p.getType() == ChessPiece:: PAWN && (coupRep.posEnd.y == 0 || coupRep.posEnd.y ==7)) {
                promotion = true;
            }else {
                plateau.allPositions.push_back(plateau.getFen());
                std::cout << "position : " << plateau.allPositions.back() << std::endl;
                turnPlayer1 = !(turnPlayer1);
            }

            plateau.coordPrisePassant = gf::Vector2i(-1); 
        }else {
            return 2;
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
        
    }

    if(promotion) {    
        if(turnPlayer1) {
            if (gf::SocketStatus::Data != client1.recvPacket(packet2)) {
                std::cerr<<"erreur lors de la réception du packet qui contient la promotion du client 1";
                return -1;
            }
        
            assert(packet2.getType() == PromotionReq::type);
            promo = packet2.as<PromotionReq>();
        }else {
            if (gf::SocketStatus::Data != client2.recvPacket(packet2)) {
                std::cerr<<"erreur lors de la réception du packet qui contient la promotion du client 2";
                return -1;
            }
            
            assert(packet2.getType() == PromotionReq::type);
            promo = packet2.as<PromotionReq>();  
        }

        PromotionRep promoRep = buildRepPromotion(plateau, gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice); 

        if(promoRep.err == CodeRep::NONE) { // coup valide
            std::cout << "------PROMO VALIDE------" << std::endl;
            
            plateau.promotionPiece(gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice);
            promotion = false;
            plateau.allPositions.push_back(plateau.getFen());
            std::cout << "position : " << plateau.allPositions.back() << std::endl;
            turnPlayer1 = !(turnPlayer1);
        }else {
            return 2;
        }

        packet2.is(promoRep);

        if (gf::SocketStatus::Data != client1.sendPacket(packet2)) {
            std::cerr<<"erreur lors de l'envoie du packet coupRep au client 1";
            return -1;
        }
        
        packet2.is(promoRep);

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


