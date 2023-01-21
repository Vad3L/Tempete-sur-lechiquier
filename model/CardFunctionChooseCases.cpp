#include "CardFunctionChooseCases.hpp"

//TO DO inclure vérification echec de Léo
bool isPossibleSwapPieces(Plateau &p,ChessPiece a, ChessColor x, ChessPiece b, ChessColor y){
	for(auto caseEchiquier: p.state){
		for(auto caseEchiquierbis: p.state){
			if(caseEchiquier.piece.getType()==a && caseEchiquier.piece.getColor()== x){
				if(caseEchiquierbis.piece.getType()==b && caseEchiquier.piece.getColor()== y){
					return true;
				}
			}
		}
	}
	return false;
}
bool checkGoodChoose(ChessPiece typeOne,ChessPiece pieceChooseOne, ChessPiece typeTwo,ChessPiece pieceChooseTwo){
	if(pieceChooseOne!= typeOne || pieceChooseOne!=typeTwo){
		return false;
	}
	if(pieceChooseTwo!= typeOne ||pieceChooseTwo!= typeTwo){
		return false;
	}
	return true;

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

void Chameau (Plateau& p, gf::Vector2i s) {
	gf::Log::info("apelle Chameau execute\n");
	inBoard(s);
	Piece &piece=p.state[s.y*8+s.x].piece;
	piece= Piece(piece.getColor(),ChessPiece::CAMEL);
	
}

bool ChameauIsPlayable (Plateau& p, Phase f,gf::Vector2i c) {
	gf::Log::info("apelle chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	return p.state[c.y*8+c.x].piece.getType()==ChessPiece::KNIGHT;

}
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle Quatre coin execute\n");
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
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

	if(checkGoodChoose(ChessPiece::ROOK,d.getType(),ChessPiece::BISHOP,c.getType())){
		
		std::swap(c,d);
	}
	

}


bool AsileIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("apelle Asile jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	return isPossibleSwapPieces(p,ChessPiece::ROOK,ChessPiece::BISHOP,p.turnTo,p.turnTo);

	return true;
}