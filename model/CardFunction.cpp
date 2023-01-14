#include "CardFunction.hpp"

void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e) {}
bool NoCardPlayable (Plateau& p, Phase f) { return false; }

void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	Case c = p.state[s.y * 8 + s.x];
	ChessColor clr = c.piece.getColor();
	c.piece = Piece(clr, ChessPiece::CAMEL);
}

bool ChameauIsPlayable (Plateau& p, Phase f) {
	if (f != Phase::APRES_COUP) {
		return false;
	}

	gf::Vector2i pos = p.lastCoup[p.lastCoup.size() - 1];
	Case c = p.state[pos.y * 8 + pos.x];
	if (c.piece.getType() == ChessPiece::KNIGHT) {
		return true;
	}
	return false;
}

void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	inBoard(e);
	Case c = p.state[s.y * 8 + s.x];
	Case d = p.state[e.y * 8 + e.x];
	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
}

bool QuatreCoinIsPlayable (Plateau& p, Phase f) {
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

