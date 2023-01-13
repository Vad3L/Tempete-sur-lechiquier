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

    std::cout << c.position.y << std::endl;
    std::cout << (piece.getColor() == ChessColor::WHITE ? "white" : "black") << std::endl;

    if (piece.getType() == ChessPiece::PAWN) {
    	if (c.position.y == 0 && piece.getColor() == ChessColor::WHITE) {
        	gf::Log::debug("Promotion Accepter du pion Blanc\n");
		    promoValide = true;
    	}

    	if (c.position.y == 7 && piece.getColor() == ChessColor::BLACK) {
       		gf::Log::debug("Promotion Accepter du pion noir\n");
    		promoValide = true;
    	}
    }

    if(!(r.choice == ChessPiece::QUEEN || r.choice == ChessPiece::ROOK || r.choice == ChessPiece::BISHOP || r.choice == ChessPiece::KNIGHT)) {
        gf::Log::error("Promotion invvalide : Type\n");
        promoValide = false;
    }

    if (promoValide) {
        r.err = CodeRep::NONE;
    } else {
        gf::Log::error("Tentative de promotion frauduleuse \n");
        r.err = CodeRep::COUP_NO_VALIDE;
    }

}

bool performCoup (Plateau& plateau, CoupRep& coup) {
	gf::Log::debug("------COUP VALIDE------\n");
    Piece p = plateau.state[coup.posStart.y * 8 + coup.posStart.x].piece;
    p.isMoved = true;
    plateau.movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));
    plateau.prettyPrint();
    plateau.lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
    plateau.lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   

	if (p.getType() == ChessPiece::PAWN && (coup.posEnd.y == 0 || coup.posEnd.y == 7)) {
		return true;
	}
	return false;
}

void performPromotion (Plateau& plateau, PromotionRep& promo) {
	gf::Log::debug("------PROMO VALIDE------\n");
    plateau.promotionPiece(gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice);
    plateau.allPositions.push_back(plateau.getFen());
    gf::Log::debug("position : ");
    std::cout << plateau.allPositions.back() << std::endl;
}

int performAction (Plateau& plateau, gf::TcpSocket& client1, gf::TcpSocket& client2, bool& turnPlayer1, bool &promotion) {
    CoupRep coup;
    PromotionRep promo;
    
    if (!promotion) {
        if (turnPlayer1) {
	        gf::Packet pack;
            if(receivingPacket(client1, pack) == -1) {
                return -1;
            }
            assert(pack.getType() == CoupRep::type);
            coup = pack.as<CoupRep>();
        } else {
            gf::Packet pack;
	        if(receivingPacket(client2, pack) == -1) {
                return -1;
            }
            assert(pack.getType() == CoupRep::type);
            coup = pack.as<CoupRep>();
        }

        checkCoupPacketValidity(plateau, coup);

        if (coup.err == CodeRep::NONE) { // coup valide
            promotion = performCoup(plateau, coup);

            if (!promotion) {
                plateau.allPositions.push_back(plateau.getFen());
                gf::Log::debug("position : ");
                std::cout <<  plateau.allPositions.back() << std::endl;
                turnPlayer1 = !(turnPlayer1);
            }

        } else {
            return 2;
        }
        
        gf::Packet pack;
        pack.is(coup);

       	if(sendingPacket(client1, pack) == -1){
            return -1;
        }
        if(sendingPacket(client2, pack) == -1) {
            return -1;
        }
    }

    if (promotion) {
        if(turnPlayer1) {
	        gf::Packet pack;
            if(receivingPacket(client1, pack) == -1) {
                return -1;
            }
            assert(pack.getType() == PromotionRep::type);
            promo = pack.as<PromotionRep>();
        } else {
            gf::Packet pack;
	        if(receivingPacket(client2, pack) == -1) {
                return -1;
            }
            assert(pack.getType() == PromotionRep::type);
            promo = pack.as<PromotionRep>();  
        }

        checkPromotionValidity(plateau, promo); 

        if(promo.err == CodeRep::NONE) { // coup valide
            performPromotion(plateau, promo);
            promotion = false;
            turnPlayer1 = !(turnPlayer1);
        } else {
            gf::Log::error("------PROMO INVALIDE------\n");
            return 2;
        }

        gf::Packet pack;
        pack.is(promo);

        if(sendingPacket(client1, pack) == -1){
            return -1;
        }
        if(sendingPacket(client2, pack) == -1) {
            return -1;
        }
    }

    return 0;
}

int sendStartOrEnd (gf::TcpSocket& a, gf::TcpSocket& b, CodeRep code, ChessStatus s, ChessColor c) {
	int ret = 0;
    gf::Packet packet;
	PartieRep rep;
	rep.err = code;
	rep.colorPion = c;
	rep.status = s;
	packet.is(rep);
    
    if (!a || !b) {
		gf::Log::error("Client déconnecté\n");
		ret = -1;
	}
    
	if (gf::SocketStatus::Data != a.sendPacket(packet)) {
		gf::Log::error("Lors de l'envoie du packet de debut ou fin au client 1\n");
        ret = -1;
	}
    
	if (gf::SocketStatus::Data != b.sendPacket(packet)) {
		gf::Log::error("Lors de l'envoi du packet de debut fin au client 2\n");
        ret = -1;
	}

    return 0;
}

