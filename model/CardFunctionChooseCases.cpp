#include "CardFunctionChooseCases.hpp"

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

void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("apelle Chameau execute\n");
	inBoard(s);
	
}

bool ChameauIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("apelle chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	
	
	return false;
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

void Exil(Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("apelle Exil execute\n");
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
	Case &d = p.state[e.y * 8 + e.x];
	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
}


bool ExilIsPlayable(Plateau& p, Phase f){
	gf::Log::info("apelle Exil jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	// a refaire todo $ :>
	return true;
}