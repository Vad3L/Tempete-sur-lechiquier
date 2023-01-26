#include "CardFunction.hpp"
#include "cassert"

//tools
std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}

bool binNotChange(Plateau& p){

	int len = p.allPositions.size();
	
	if (len - 2 < 0) {
		return false;
	}

	std::string before = p.allPositions[len - 2];
	std::string now = p.allPositions.back();
	std::string not_piece = "1234567890/";


	if (len >= 3){
		int index = (p.turnTo == ChessColor::WHITE ? 0:7 );
		char c = (p.turnTo == ChessColor::WHITE ? 'P':'p' );
		auto tabS = explode(before,'/');
		if(tabS[index].find(c,0) != std::string::npos){
			before = p.allPositions[len - 3];
		}
	}

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

bool isInEchecAfterCardGreen(Plateau &p , std::function<bool(Plateau&, std::vector<gf::Vector2i> tabVector)> execute) {
	Plateau pp = p;
	
	assert(pp.getFen() == p.getFen());

	execute(pp, {});

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

	return ret ;
}

//card
bool NoCard (Plateau& p, std::vector<gf::Vector2i> tabVector) { 
	if(tabVector.size() != 0 || p.turnTo == ChessColor::GREY){exit(8);}
	return false;
}
bool NoCardPlayable (Plateau& p, Phase f) { 
	if(p.turnTo == ChessColor::GREY){exit(7);}
	return false; 
}

bool Princess (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Apelle Princesse execute\n");
	if(tabVector.size() != 0){return false;}
	gf::Vector2i pos = (p.turnTo  == ChessColor::WHITE ? gf::Vector2i(3,7): gf::Vector2i(3,0)); 
	Case &c = p.state[pos.y * 8 + pos.x];
	c.piece = Piece(p.turnTo, ChessPiece::PRINCESS);

	return true;
}

bool PrincessIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appel Princesse jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}
	
	if (isInEchecAfterCardGreen(p, Princess)){
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

bool BombeAtomique (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Bombe atomique execute\n");
	if(tabVector.size() != 0){return false;}
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

	return true;
}

bool BombeAtomiqueIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appel Bombe atomique jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	if (isInEchecAfterCardGreen(p, BombeAtomique)){
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}
	
	return binNotChange(p);
}

bool Vampirisme (Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Vampirisme execute\n");
	if(tabVector.size() != 0){return false;}
	gf::Vector2i pos = p.lastCoup.back();
	p.state[pos.y * 8 + pos.x].piece = Piece(p.turnTo, p.bin.back().getType());

	return true;
}

bool VampirismeIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appel Vampirisme jouable\n");

	if (f != Phase::APRES_COUP || binNotChange(p)) {
		return false;
	}
	
	if (isInEchecAfterCardGreen(p, Vampirisme)){
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}

	gf::Vector2i pos = p.lastCoup.back();
	
	return p.state[pos.y * 8 + pos.x].piece.getType() != ChessPiece::KING;
}

bool VisitesOfficielles (Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Visites Officielles execute\n");
	if(tabVector.size() != 0){return false;}
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

	return true;
}

bool VisitesOfficiellesIsPlayable (Plateau& p, Phase f){
	gf::Log::info("Appel Visites Officielles jouable\n");
	if (f != Phase::APRES_COUP ){ 
		return false;
	}

	if(isInEchecAfterCardGreen(p, VisitesOfficielles)) {
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		return false;
	}

	return true;

}