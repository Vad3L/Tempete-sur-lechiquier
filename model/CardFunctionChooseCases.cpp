#include "CardFunctionChooseCases.hpp"

bool isInEchecAfterCard(Plateau &p , std::function<bool(Plateau&, gf::Vector2i s, gf::Vector2i e)> execute) {
	Plateau pp = p;
	
	assert(pp.getFen() == p.getFen());

	execute(pp,gf::Vector2i(-1),gf::Vector2i(-1));

	bool ret = true;
	gf::Vector2i caseProvocateEchec(-1);
	if(p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
	}

	// chercher si une autre case case met en echec l'adversaire apres l'activation de la carte
	if(p.turnTo == ChessColor::WHITE) {
		ret = pp.isInEchec(ChessColor::WHITE) || pp.isInEchec(ChessColor::BLACK, gf::Vector2i(-1), caseProvocateEchec);
	}else {
		ret = pp.isInEchec(ChessColor::WHITE, gf::Vector2i(-1), caseProvocateEchec) || pp.isInEchec(ChessColor::BLACK);
	}

	return ret;
}


//Ne fonctionne que pour des pièces de même couleurs
bool isPossibleSwapPieces(Plateau &p,ChessPiece p1, ChessPiece p2, ChessColor color){
	for(auto caseEchiquier: p.state){
		for(auto caseEchiquierbis: p.state){
			if(checkGoodChoose(p, p1, caseEchiquier.piece, p2, caseEchiquierbis.piece, color)){
				return true;
			}
		}
	}
	return false;
}

bool checkGoodChoose(Plateau &p,ChessPiece typeOne,Piece &pieceChooseOne, ChessPiece typeTwo,Piece &pieceChooseTwo,ChessColor color){
	bool res = false;
	
	if(pieceChooseOne.getType()== typeOne && pieceChooseTwo.getType()==typeTwo && pieceChooseOne.getColor() == color && pieceChooseTwo.getColor() == color){
		res=true;
	}
	
	if(pieceChooseOne.getType()== typeTwo && pieceChooseTwo.getType()== typeOne && pieceChooseTwo.getColor() == color && pieceChooseOne.getColor() == color){
		res=true;
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
	
	Piece &p1 = p.state[s.y * 8 + s.x].piece;
	Piece &p2 = p.state[e.y * 8 + e.x].piece;

	if(checkGoodChoose(p, ChessPiece::KNIGHT, p1, ChessPiece::BISHOP, p2, p.turnTo)) {
		std::swap(p1, p2);
		return true;
	}

	return false;
}

bool ChevalFouIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle ChevalFou jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return isPossibleSwapPieces(p, ChessPiece::KNIGHT, ChessPiece::BISHOP, p.turnTo);
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

	piece = Piece(playerColor, ChessPiece::CAMEL);

	if (p.isInEchec(ChessColor::WHITE) || p.isInEchec(ChessColor::BLACK)) {
		piece = Piece(playerColor, ChessPiece::KNIGHT);
		return false;
	}
	return true;
}

bool ChameauIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	return true;

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

	if (occupied != 3) {
		return false;
	}

	return true;
}

bool Asile(Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("apelle Asile execute\n");
	
	inBoard(s);
	inBoard(e);
	Piece &c = p.state[s.y * 8 + s.x].piece;
	Piece &d = p.state[e.y * 8 + e.x].piece;
	if(checkGoodChoose(p,ChessPiece::ROOK,d,ChessPiece::BISHOP,c,p.turnTo)){
		gf::Log::info("Check true\n");
		std::swap(c,d);

	}
	
	return true;
}


bool AsileIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("apelle Asile jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	return isPossibleSwapPieces(p,ChessPiece::ROOK,ChessPiece::BISHOP,p.turnTo);

	return true;
}
