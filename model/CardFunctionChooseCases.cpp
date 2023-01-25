#include "CardFunctionChooseCases.hpp"

//Ne fonctionne que pour des pièces de même couleurs
bool isPossibleSwapPieces(Plateau &p,ChessPiece a, ChessPiece b, ChessColor color){
	for(auto caseEchiquier: p.state){
		for(auto caseEchiquierbis: p.state){
			if(checkGoodChoose(p,a,caseEchiquier.piece,b,caseEchiquierbis.piece,color)){
				return true;
			}
		}
	}
	return false;
}

bool checkGoodChoose(Plateau &p,ChessPiece typeOne,Piece pieceChooseOne, ChessPiece typeTwo,Piece pieceChooseTwo,ChessColor color){
	bool res=true;
	if(!(pieceChooseOne.getType()== typeOne && pieceChooseTwo.getType()==typeTwo) && pieceChooseOne.getColor() != color && pieceChooseTwo.getColor() != color){
		gf::Log::info("false 1, color %i\n",(int)(color));
		res=false;
	}
	if(!(pieceChooseOne.getType()== typeTwo && pieceChooseTwo.getType()== typeOne) && pieceChooseTwo.getColor() != color && pieceChooseOne.getColor() != color){
		gf::Log::info("False 2, color %i\n",(int)(color));
		res=false;
	}else{
		res=true;
	}

	if(!res){
		return false;
	}

	std::swap(pieceChooseOne,pieceChooseTwo);
	
	if(p.isInEchec(ChessColor::WHITE) || p.isInEchec(ChessColor::BLACK)){
		res=false;
	}

	std::swap(pieceChooseTwo,pieceChooseOne);
	return res;

}

void ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle ChevalFou execute\n");
	inBoard(s);
	inBoard(e);

	Piece temp = p.state[s.y * 8 + s.x].piece;
	p.state[s.y * 8 + s.x].piece = p.state[e.y * 8 + e.x].piece;
	p.state[e.y * 8 + e.x].piece = temp;
}

bool ChevalFouIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle ChevalFou jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	return true;
}

void Chameau (Plateau& p, gf::Vector2i s,gf::Vector2i e) {
	gf::Log::info("apelle Chameau execute\n");
	inBoard(s);
	Piece &piece=p.state[s.y*8+s.x].piece;
	piece= Piece(piece.getColor(),ChessPiece::CAMEL);
	
}

bool ChameauIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return true;

}
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle Quatre coin execute\n");
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
	// todo ne pas utiliser e mais cherhce le quatrième coins vide
	Case &d = p.state[e.y * 8 + e.x];
	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
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

void Asile(Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("apelle Asile execute\n");
	
	inBoard(s);
	inBoard(e);
	Piece &c = p.state[s.y * 8 + s.x].piece;
	Piece &d = p.state[e.y * 8 + e.x].piece;
	if(checkGoodChoose(p,ChessPiece::ROOK,d,ChessPiece::BISHOP,c,p.turnTo)){
		gf::Log::info("Check true\n");
		std::swap(c,d);

	}
	

}


bool AsileIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("apelle Asile jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	return isPossibleSwapPieces(p,ChessPiece::ROOK,ChessPiece::BISHOP,p.turnTo);

	return true;
}