#include "CardFunctionChooseCases.hpp"

bool pieceExist(Plateau &p,ChessPiece type, ChessColor color) {
	for(auto caseEchiquier: p.state){
		if(caseEchiquier.piece.getType() == type && caseEchiquier.piece.getColor() == color) {
			return true;
		}
	}
	return false;
}

bool checkGoodCase(Plateau &p, gf::Vector2i coord, ChessPiece type, ChessColor color) {
	Piece &piece = p.state[coord.y * 8 + coord.x].piece;
	return piece.getType() == type &&  piece.getColor() == color;
}

bool checkGoodChoose(Plateau &p, Piece &pieceChooseOne, Piece refA , Piece &pieceChooseTwo, Piece refB) {
	bool res = false;

	if(refA == pieceChooseOne && refB == pieceChooseTwo) {
		res = true;
	} 

	if(refA == pieceChooseTwo && refB == pieceChooseOne) {
		res = true;
	} 

	if(!res){
		return false;
	}
	
	std::swap(pieceChooseOne, pieceChooseTwo);

	gf::Vector2i caseProvocateEchec(-1);
	if(p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
	}

	// chercher si une autre case case met en echec l'adversaire apres l'activation de la carte
	if(p.turnTo == ChessColor::WHITE) {
		res = p.isInEchec(ChessColor::WHITE) || p.isInEchec(ChessColor::BLACK, gf::Vector2i(-1), caseProvocateEchec);
	}else {
		res = p.isInEchec(ChessColor::WHITE, gf::Vector2i(-1), caseProvocateEchec) || p.isInEchec(ChessColor::BLACK);
	}
	
	std::swap(pieceChooseTwo, pieceChooseOne);
	return !res;
}

bool ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle ChevalFou execute\n");
	if(!inBoard(s) || !inBoard(e)) {
		return false;
	}
	
	Piece &piece1 = p.state[s.y * 8 + s.x].piece;
	Piece &piece2 = p.state[e.y * 8 + e.x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KNIGHT), piece2, Piece(p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool ChevalFouIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle ChevalFou jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) || pieceExist(p, ChessPiece::BISHOP, p.turnTo);
}


bool Chameau (Plateau& p, gf::Vector2i s,gf::Vector2i e) {
	gf::Log::info("apelle Chameau execute\n");
	if(!inBoard(s)) {
		return false;
	}
	Piece& piece=p.state[s.y * 8 + s.x].piece;

	if (piece.getType() != ChessPiece::KNIGHT) {
		return false;
	}
	ChessColor playerColor = piece.getColor();

	gf::Vector2i caseProvocateEchec(-1);
	if (p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec;
	}

	piece = Piece(playerColor, ChessPiece::CAMEL);

	bool res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if (res) {
		piece = Piece(playerColor, ChessPiece::KNIGHT);
	}
	return !res;
}

bool ChameauIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo);
}


bool QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle Quatre coin execute\n");
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
	// todo ne pas utiliser e mais cherhce le quatrième coins vide
	Case &d = p.state[e.y * 8 + e.x];
	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);

	return true;
}

bool QuatreCoinIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle Quatre coin jouable\n");
	if (f != Phase::AVANT_COUP) {
		return false;
	}

	int occupied = 0;
	std::vector<gf::Vector2i> coins = { 	gf::Vector2i(0, 0),
						gf::Vector2i(0, 7),
						gf::Vector2i(7, 0),
						gf::Vector2i(7, 7) };
	for (auto c : coins) {
		if (p.state[c.y * 8 + c.x].piece.getType() != ChessPiece::NONE) {
			occupied++;
		}
	}

	return occupied == 3;
}

bool Asile(Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("apelle Asile execute\n");
	
	if(!inBoard(s) || !inBoard(e)) {
		return false;
	}
	
	Piece &piece1 = p.state[s.y * 8 + s.x].piece;
	Piece &piece2 = p.state[e.y * 8 + e.x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::ROOK), piece2, Piece(p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}


bool AsileIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("apelle Asile jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::ROOK, p.turnTo) || pieceExist(p, ChessPiece::BISHOP, p.turnTo);
}

