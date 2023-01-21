#include "CardFunction.hpp"
#include "cassert"

//tools
bool binNotChange(Plateau& p){

	int len = p.allPositions.size();
	
	if (len - 2 < 0) {
		return false;
	}

	std::string before = p.allPositions[len - 2];
	std::string now = p.allPositions.back();
	std::string not_piece = "1234567890/";

	size_t count_before = 0;
	size_t count_after = 0;

	for (auto c : before) {
		if (not_piece.find(c, 0) == std::string::npos) {
			count_before++;
		}
	}

	for (auto c : now) {
		if (not_piece.find(c, 0) == std::string::npos) {
			count_after++;
		}
	}

	if (count_before == count_after) {
		return true;
	}
	return false;
}

bool isInEchecAfterCard(Plateau &p , std::function<void(Plateau&, gf::Vector2i s, gf::Vector2i e)> execute) {
	Plateau pp = p;
	assert(pp.getFen() == p.getFen());
	// achanger plus tard pour mettre s et e
	execute(pp,gf::Vector2i(-1),gf::Vector2i(-1));
	
	return pp.isInEchec(ChessColor::WHITE) || pp.isInEchec(ChessColor::BLACK);
}

//card
void NoCard (Plateau& p, gf::Vector2i s, gf::Vector2i e) {}
bool NoCardPlayable (Plateau& p, Phase f) { return false; }

void Princess (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("Apelle Princesse execute\n");
	gf::Vector2i pos = (p.turnTo  == ChessColor::WHITE ? gf::Vector2i(3,7): gf::Vector2i(3,0)); 
	Case &c = p.state[pos.y * 8 + pos.x];
	c.piece = Piece(p.turnTo, ChessPiece::PRINCESS);
}

bool PrincessIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Apellle Princesse jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	
	if (isInEchecAfterCard(p, Princess)){
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}

	char to_find = (p.turnTo  == ChessColor::WHITE ? 'Q' : 'q');
	gf::Vector2i pos = (p.turnTo  == ChessColor::WHITE ? gf::Vector2i(3,7): gf::Vector2i(3,0)); 
	std::string lastBoard = p.getFen();

	size_t res = lastBoard.find(to_find); 

	if (res == std::string::npos) {
		if (p.state[pos.y * 8 + pos.x].piece.getType() == ChessPiece::NONE){
			return true;
		}
	} 
	return false;
}

void BombeAtomique (Plateau& p, gf::Vector2i s, gf::Vector2i e) {
	gf::Log::info("Appelle Bombe atomique execute\n");

	std::vector<gf::Vector2i> targets;

	assert(p.lastCoup.size()>1);
	gf::Vector2i pos = p.lastCoup.back();

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (inBoard({pos.x+i, pos.y+j})) {
				//gf::Log::info("la : i = %i et j = %i\n", i, j);
				targets.push_back(gf::Vector2i(pos.x+i, pos.y+j));
			}
		}
	}

	for (auto& v : targets) {
		if(p.state[v.y * 8 + v.x].piece.getType() != ChessPiece::KING && p.state[v.y * 8 + v.x].piece.getType() != ChessPiece::NONE) {
			Piece piece = p.state[v.y * 8 + v.x].piece;
			p.bin.push_back(piece);
			p.state[v.y * 8 + v.x].piece = Piece(ChessColor::NONE, ChessPiece::NONE);
		}
	}
}

bool BombeAtomiqueIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appelle Bombe atomique jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	if (isInEchecAfterCard(p, BombeAtomique)){
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}
	
	return binNotChange(p);
}

void Vampirisme (Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("Appelle Vampirisme execute\n");

	gf::Vector2i pos = p.lastCoup.back();
	p.state[pos.y * 8 + pos.x].piece = Piece(p.turnTo, p.bin.back().getType());
}

bool VampirismeIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appelle Vampirisme jouable\n");

	if (f != Phase::APRES_COUP || binNotChange(p)) {
		return false;
	}
	
	if (isInEchecAfterCard(p, Vampirisme)){
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}

	gf::Vector2i pos = p.lastCoup.back();
	
	return p.state[pos.y * 8 + pos.x].piece.getType() != ChessPiece::KING;
	
}

void VisitesOfficielles (Plateau& p, gf::Vector2i s, gf::Vector2i e){
	gf::Log::info("Appelle Visites Officielles execute\n");
	gf::Vector2i roiB(-1);
	gf::Vector2i roiN(-1);

	for(auto &caseEchiquier: p.state){
		if(caseEchiquier.piece.getType()==ChessPiece::KING && caseEchiquier.piece.getColor() == ChessColor::WHITE){
			roiB = caseEchiquier.position;
		}
		if(caseEchiquier.piece.getType()==ChessPiece::KING && caseEchiquier.piece.getColor() == ChessColor::BLACK){
			roiN = caseEchiquier.position;
		}
	}

	std::swap(p.state[roiB.y*8+roiB.x].piece, p.state[roiN.y*8+roiN.x].piece);
}

bool VisitesOfficiellesIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appelle Visites Officielles jouable\n");
	if (f != Phase::APRES_COUP ){ 
		return false;
	}

	if(isInEchecAfterCard(p, VisitesOfficielles)) {
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}

	return true;

}