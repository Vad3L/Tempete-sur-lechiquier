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

void checkCardPacketValidity (Plateau& p, CardRep& r, std::vector<Card>& hand, Phase f) {
	gf::Log::debug("debut fonction card validity %i\n", r.err);
	if (r.err == CodeRep::NO_CARD) {
		return;
	}
	gf::Log::debug("j'ai recu une carte %i\n", r.err);

	assert(r.err != CodeRep::NO_CARD);
	assert(r.card <= 4);
	assert(r.card >= 0);
	assert(r.err == CodeRep::CARD);

	r.num = hand[r.card].m_num;
	bool valide = true;
	if (!hand[r.card].m_isPlayable(p, f)) {
		gf::Log::debug("carte non jouable\n");
		valide = false;
	}
	
	if (valide) {
		r.err = CodeRep::NONE;
	} else {
		r.err = CodeRep::CARD_NO_VALIDE;
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

	plateau.playerInEchec = plateau.isInEchec(!plateau.turnTo);
	
	plateau.prettyPrint();
	plateau.lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
	plateau.lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   
	gf::Log::debug("------COUP -----------------------------------------VALIDE------ %li\n", plateau.allPositions.size());
	plateau.allPositions.push_back(plateau.getFen());
	
	if (p.getType() == ChessPiece::PAWN && (coup.posEnd.y == 0 || coup.posEnd.y == 7)) {
		return true;
	}
	return false;
}

void performPromotion (Plateau& plateau, PromotionRep& promo) {
	gf::Log::debug("------PROMO VALIDE------\n");
	plateau.promotionPiece(gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice);
	
	plateau.playerInEchec = plateau.isInEchec(!plateau.turnTo);
	
	gf::Log::debug("------PROMO -----------------------------------------VALIDE------%li\n", plateau.allPositions.size());
	plateau.allPositions.push_back(plateau.getFen());
	plateau.prettyPrint();
}

void performCard (Plateau& plateau, CardRep& c, std::vector<Card>& hand, Deck& d) {
	gf::Log::debug("------CARD VALIDE------\n");
	hand[c.card].m_execute(plateau, c.a, c.b);
	plateau.allPositions.push_back(plateau.getFen());

	plateau.playerInEchec = plateau.isInEchec(!plateau.turnTo);

	gf::Log::debug("La carte %s est joué \n", hand[c.card].m_name.c_str());
	plateau.prettyPrint();
	Card pick = d.getFirst();
	d.drop(hand[c.card]);
	hand[c.card] = pick;

	if (d.getNbCardInDeck() == 0) {
		d.emptyDiscard();
	}
}

void sendHand (gf::TcpSocket& player, Card new_card) {
	gf::Packet pack;
	std::vector<Card> new_hand = { new_card };
	DistribRep rep;
	rep.err = NONE;
	rep.hand = new_hand;
	pack.is(rep);
	sendingPacket(player, pack);
}

int performTurn (Deck& d, GamePhase& gp, Plateau& p, gf::TcpSocket& player, gf::TcpSocket& other, std::vector<Card>& hand, bool& promotion) {
	gf::Log::debug("début phase %i\n", (int)(gp.getCurrentPhase()));
	gf::Packet pack;
	if (receivingPacket(player, pack) == -1) {
		gf::Log::error("Erreur lors du reçu de la carte avant le coup\n");
		return -1;
	}

	if (gp.getCurrentPhase() == Phase::AVANT_COUP && pack.getType() == CoupRep::type) {
		gp.setCurrentPhase(Phase::COUP);
	}

	if (gp.getCurrentPhase() == Phase::AVANT_COUP) {
		gf::Log::debug("Phase avant coup\n");
		assert(pack.getType() == CardRep::type);
		CardRep card = pack.as<CardRep>();
		checkCardPacketValidity(p, card, hand, gp.getCurrentPhase());
		if (card.err == CodeRep::NONE) {
			performCard(p, card, hand, d);
			sendHand(player, hand[card.card]);
			gp.nextPhaseCard(hand[card.card]);
		} else {
			return 2;
		}
		pack.is(card);
		if (sendingPacket(player, pack)) { return -1; }
		if (sendingPacket(other, pack)) { return -1; }
	} else if (gp.getCurrentPhase() == Phase::COUP) {
		if (promotion) {
			gf::Log::debug("Promotion\n");
			assert(pack.getType() == PromotionRep::type);
			PromotionRep promo = pack.as<PromotionRep>();
			checkPromotionValidity(p, promo);
			if (promo.err != CodeRep::NONE) {
				return 2;
			}
			performPromotion(p, promo);
			gp.setCurrentPhase(Phase::APRES_COUP);
			promotion = false;
			pack.is(promo);
			if (sendingPacket(player, pack)) { return -1; }
			if (sendingPacket(other, pack)) { return -1; }
		} else {
			assert(pack.getType() == CoupRep::type);
			gf::Log::debug("Coup\n");
			CoupRep coup = pack.as<CoupRep>();
			checkCoupPacketValidity(p, coup);
			if (coup.err != CodeRep::NONE) {
				return 2;
			}

			promotion = performCoup(p, coup);
		
			if (!promotion) {
				gp.setCurrentPhase(Phase::APRES_COUP);
			}
			pack.is(coup);
			if (sendingPacket(player, pack)) { return -1; }
			if (sendingPacket(other, pack)) { return -1; }
		}
	} else if (gp.getCurrentPhase() == Phase::APRES_COUP) {
		assert(pack.getType() == CardRep::type);
		gf::Log::debug("Apres coup!\n");
		CardRep card = pack.as<CardRep>();
		checkCardPacketValidity(p, card, hand, gp.getCurrentPhase());
		if (card.err == CodeRep::NONE) {
			performCard(p, card, hand, d);
			sendHand(player, hand[card.card]);
			gp.nextPhaseCard(hand[card.card]);
		} else if (card.err == CodeRep::NO_CARD) {
			gp.setCurrentPhase(Phase::PAS_MON_TOUR);
		} 

		pack.is(card);
		if(card.err != CodeRep::NO_CARD) {
			if (sendingPacket(player, pack)) { return -1; }
			if (sendingPacket(other, pack)) { return -1; }
		}
		
	}

	if (gp.getCurrentPhase() == Phase::PAS_MON_TOUR) {
		gf::Log::debug("tour finito\n");
		return 0;
	}
	return 2;
}
/*
int performTurn (Plateau& plateau, gf::TcpSocket& player, gf::TcpSocket& other, std::vector<Card>& hand,  bool& promotion) {
	gf::Packet pack;
	if (receivingPacket(player, pack) == -1) {
		gf::Log::error("Erreur lors du reçu de la carte avant le coup\n");
		return -1;
	}

	if (pack.getType() == CardRep::type && !promotion) {
		CardRep card = pack.as<CardRep>();
		checkCardPacketValidity(plateau, card, hand, Phase::AVANT_COUP);
		if (card.err == CodeRep::NONE) {
			performCard(plateau, card, hand);
			if (hand[card.card].m_effect == Effect::REPLACE_COUP) {
				return 0;
			}
		}else {
			return 2;
		}
		pack.is(card);
		// send to both
		if (sendingPacket(player, pack) == -1) { return -1; }
		if (sendingPacket(other, pack) == -1) { return -1; }
	}

	if (pack.getType() == CoupRep::type && !promotion) {
		CoupRep coup = pack.as<CoupRep>();
		checkCoupPacketValidity(plateau, coup);
		if (coup.err != CodeRep::NONE) {
			return 2;
		}
		promotion = performCoup(plateau, coup);
		pack.is(coup);
		// send to both
		if (sendingPacket(player, pack) == -1) { return -1; }
		if (sendingPacket(other, pack) == -1) { return -1; }
		if (promotion) {
			return 2;
		}
	} else if (pack.getType() == PromotionRep::type) {
		PromotionRep promo = pack.as<PromotionRep>();
		checkPromotionValidity(plateau, promo);
		if (promo.err != CodeRep::NONE) {
			return 2;
		}
		performPromotion(plateau, promo);
		promotion = false;
		pack.is(promo);
		// send to both
		if (sendingPacket(player, pack) == -1) { return -1; }
		if (sendingPacket(other, pack) == -1) { return -1; }
	}

	gf::Packet pack2;
	if (receivingPacket(player, pack2) == -1) {
		gf::Log::error("Erreur lors du reçu de la carte après le coup");
		return -1;
	}

	assert(pack2.getType() == CardRep::type);
	
	CardRep card = pack2.as<CardRep>();
	checkCardPacketValidity(plateau, card, hand, Phase::APRES_COUP);

	if (card.err == CodeRep::NONE) {
		performCard(plateau, card, hand);
	}
	pack2.is(card);

	if (sendingPacket(player, pack2) == -1) {
		return -1;
	}
	if (sendingPacket(other, pack2) == -1) {
		return -1;
	}
	return 0;
}
*/
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

	return ret;
}

int sendStartTurn (gf::TcpSocket& a){
	int ret = 0;
	gf::Packet packet;

	PartieRep debut;
	debut.err = TURN_START;
	packet.is(debut);

	if (gf::SocketStatus::Data != a.sendPacket(packet)) {
		gf::Log::error("Lors de l'envoie du packet de debut de tour\n");
		ret = -1;
	}

	return ret;
}

int sendInit(gf::TcpSocket& client, ChessColor c, std::vector<Card>& hand) {
	gf::Packet pack;
		
		PartieRep rep;
		rep.err = COLOR;
		rep.colorPion = c;

		pack.is(rep);
		if (gf::SocketStatus::Data != client.sendPacket(pack)) {
			gf::Log::error("Lors de l'envoie du packet contenant la couleur au client\n");
			exit(-2);
		}

		DistribRep distrib;
		distrib.err = NONE;
		distrib.hand = hand;
		pack.is(distrib);
		if (gf::SocketStatus::Data != client.sendPacket(pack)) {
			gf::Log::error("Lors de l'envoie du packet contenant la main au client\n");
			exit(-1);
		}
	return 0;
}

