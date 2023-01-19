#include "CardFunction.hpp"

void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e) {}
bool NoCardPlayable (Plateau& p, Phase f) { return false; }

void Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	//todo
}

bool PrincessIsPlayable (Plateau& p, Phase f) {
	if (f != Phase::APRES_COUP) {
		return false;
	}
	//todo
	
	return false;
}


void Chameau (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	gf::Vector2i pos = p.lastCoup[p.lastCoup.size() - 1];

	Case &c = p.state[pos.y * 8 + pos.x];
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

void BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	std::vector<gf::Vector2i> targets;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i >= 0 && i < 8 && j >= 0 && j < 8) {
				targets.push_back(gf::Vector2i(i, j));
			}
		}
	}

	for (auto& v : targets) {
		p.state[v.y * 8 + v.x].piece = Piece(ChessColor::NONE, ChessPiece::NONE);
	}
}

bool BombeAtomiqueIsPlayable (Plateau& p, Phase f) {
	size_t len = p.allPositions.size();
	std::string before = p.allPositions[len - 2];
	std::string now = p.allPositions[len - 1];
	std::string not_piece = "1234567890/";

	size_t count_before = 0;
	size_t count_after = 0;

	for (auto& c : before) {
		if (not_piece.find_first_of(c, 0) == std::string::npos) {
			count_before++;
		}
	}

	for (auto& c : now) {
		if (not_piece.find_first_of(c, 0) == std::string::npos) {
			count_after++;
		}
	}

	if (count_before > count_after) {
		return true;
	}

	return false;
}

void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
	Case &d = p.state[e.y * 8 + e.x];
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




void Exil(Plateau& p, gf::Vector2i s, gf::Vector2i e){
	inBoard(s);
	inBoard(e);
	Case &c = p.state[s.y * 8 + s.x];
	Case &d = p.state[e.y * 8 + e.x];
	d.piece = c.piece;
	c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
}


bool ExilIsPlayable(Plateau& p, Phase f){
	if(f!= Phase::APRES_COUP){
		return false;
	}

	gf::Vector2i pos = p.lastCoup[p.lastCoup.size() - 1];
	Case c = p.state[pos.y * 8 + pos.x]; //get the type of the piece
	ChessColor clr = c.piece.getColor();
	switch(c.piece.getType()){

		case ChessPiece::KING : //If the WHITE king is choose, check if his first emplacement is available else we check for the Black king
			if(clr == ChessColor::WHITE){
				if(p.state[0+4].piece.getType()!= ChessPiece::NONE){
					return false;
				}else{
					if(p.state[7*8+4].piece.getType()!= ChessPiece::NONE){
						return false;
					}
				}
			}
		break;

		case ChessPiece::QUEEN :
			if(clr == ChessColor::WHITE){
				if(p.state[0+3].piece.getType()!= ChessPiece::NONE){
					return false;
				}else{
					if(p.state[7*8+3].piece.getType()!= ChessPiece::NONE){
						return false;
					}
				}
			}
		break;

		case ChessPiece::BISHOP:
			//Vérifier si la case est noir ou blanche pour savoir de quel fou il s'agit et savoir si la pièce est blanche ou noir
		break;

		case ChessPiece::CAMEL:
		case ChessPiece::KNIGHT:
			if(clr == ChessColor::WHITE){

				std::vector<gf::Vector2i> knightMovement = { 	gf::Vector2i(0, 1),
					gf::Vector2i(2, 0),
					gf::Vector2i(2, 2),
					gf::Vector2i(0, 6),
					gf::Vector2i(2, 7),
					gf::Vector2i(2, 5)};

				for (auto k : knightMovement) {
					if (p.state[k.y * 8 + k.x].piece.getType() == ChessPiece::NONE) {
						return true;
					}
				}
			}else{
				std::vector<gf::Vector2i> knightMovement = { 	gf::Vector2i(7, 1),
					gf::Vector2i(5, 0),
					gf::Vector2i(5, 2),
					gf::Vector2i(7, 6),
					gf::Vector2i(5, 7),
					gf::Vector2i(5, 5)};

				for (auto k : knightMovement) {
					if (p.state[k.y * 8 + k.x].piece.getType() == ChessPiece::NONE) {
						return true;
					}
				}

			}
		break;

		case ChessPiece::ROOK:
			if(clr == ChessColor::WHITE){
				if(p.state[0+0].piece.getType()!= ChessPiece::NONE || p.state[0+7].piece.getType()!= ChessPiece::NONE){
					return false;
				}
			}else{
				if(p.state[7*8+0].piece.getType()!= ChessPiece::NONE || p.state[7*8+7].piece.getType()!= ChessPiece::NONE){
					return false;
				}	
			}
		break;

		case ChessPiece::PAWN:
			if(clr == ChessColor::WHITE){
				std::vector<gf::Vector2i> pawnMovement;
				for(auto i = 0; i<7;++i){
					for(auto j = 1; i<3; ++j){
						pawnMovement.push_back({i,j});
					}
				}
				for (auto pawn : pawnMovement) {
					if (p.state[pawn.y * 8 + pawn.x].piece.getType() == ChessPiece::NONE) {
						return true;
					}
				}

			}else{
				std::vector<gf::Vector2i> pawnMovement;
				for(auto i = 0; i<7;++i){
					for(auto j = 6; i>4; --j){
						pawnMovement.push_back({i,j});
					}
				}
				for (auto pawn : pawnMovement) {
					if (p.state[pawn.y * 8 + pawn.x].piece.getType() == ChessPiece::NONE) {
						return true;
					}
				}
			}
		break;
	}
	return true;
}



