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
	/*if(p.turnTo == ChessColor::WHITE) {
		res = p.isInEchec(ChessColor::WHITE) || p.isInEchec(ChessColor::BLACK, gf::Vector2i(-1), caseProvocateEchec);
	}else {
		res = p.isInEchec(ChessColor::WHITE, gf::Vector2i(-1), caseProvocateEchec) || p.isInEchec(ChessColor::BLACK);
	}*/
	res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo, gf::Vector2i(-1), caseProvocateEchec);

	std::swap(pieceChooseTwo, pieceChooseOne);
	return !res;
}

bool ChevalFou (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel ChevalFou execute\n");
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KNIGHT), piece2, Piece(p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool ChevalFouIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel ChevalFou jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) && pieceExist(p, ChessPiece::BISHOP, p.turnTo);
}


bool Chameau (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Chameau execute\n");
	if(tabVector.size() != 1 || !inBoard(tabVector[0])) {
		return false;
	}
	Piece& piece=p.state[tabVector[0].y * 8 + tabVector[0].x].piece;

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
	gf::Log::info("Appel chameau jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo);
}


bool QuatreCoin (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Quatre coin execute\n");
	if(tabVector.size() != 1 || !inBoard(tabVector[0])){
		return false;
	}
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;

	if(piece1.getType() == ChessPiece::NONE || piece1.getColor() != p.turnTo) {
		return false;
	}

	gf::Vector2i coordLibre(-1);
	std::vector<gf::Vector2i> coins = { gf::Vector2i(0, 0),
										gf::Vector2i(0, 7),
										gf::Vector2i(7, 0),
										gf::Vector2i(7, 7) };

	for (auto c : coins) {
		if (p.state[c.y * 8 + c.x].piece.getType() == ChessPiece::NONE) {
			coordLibre = c;
			break;
		}
	}
	
	if(!inBoard(coordLibre)){return false;}

	Piece &piece2 = p.state[coordLibre.y * 8 + coordLibre.x].piece;

	std::swap(piece1, piece2);

	bool res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo);
	if (res) {
		std::swap(piece1, piece2);
	}

	return !res;
}

bool QuatreCoinIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel Quatre coin jouable\n");

	if (f != Phase::AVANT_COUP  || p.playerInEchec) {
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

bool Asile(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Asile execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::ROOK), piece2, Piece(p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}


bool AsileIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("Appel Asile jouable\n");
	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::ROOK, p.turnTo) && pieceExist(p, ChessPiece::BISHOP, p.turnTo);
}

bool TourFada (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Tour Fada execute\n");
	if (tabVector.size() != 2) {
		return false;
	}

	if (!inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}

	Piece& piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece& piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if (checkGoodChoose(p, piece1, Piece(!p.turnTo, ChessPiece::ROOK), piece2, Piece(!p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}
	return false;
}

bool TourFadaIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel Tour Fada jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::ROOK, !p.turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.turnTo);
}

bool Urbanisme(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Urbansime execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::ROOK), piece2, Piece(!p.turnTo, ChessPiece::ROOK))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool UrbanismeIsPlayable(Plateau& p, Phase f) { 
	gf::Log::info("Appel Urbanisme jouable\n");

	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::ROOK, p.turnTo) && pieceExist(p, ChessPiece::ROOK, !p.turnTo);
}

bool Schizophrenie(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Schizophrenie execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::BISHOP), piece2, Piece(!p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool SchizophrenieIsPlayable(Plateau& p, Phase f) { 
	gf::Log::info("Appel Schizophrenie jouable\n");

	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::BISHOP, p.turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.turnTo);
}

bool BonnesCopines(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel BonnesCopines execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::QUEEN), piece2, Piece(!p.turnTo, ChessPiece::QUEEN))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}


bool BonnesCopinesIsPlayable(Plateau& p, Phase f){ 
	gf::Log::info("Appel BonnesCopines jouable\n");
	if(f!= Phase::AVANT_COUP || p.playerInEchec){
		return false;
	}

	return pieceExist(p, ChessPiece::QUEEN, p.turnTo) && pieceExist(p, ChessPiece::QUEEN, !p.turnTo);
}

bool Desintegration (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Désintégration execute\n");
	if (tabVector.size() != 1) {
		return false;
	}

	if (tabVector.size() != 1 || !inBoard(tabVector[0])) {
		return false;
	}

	Piece& piece = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	if (piece.getColor() != p.turnTo) {
		return false;
	}
	if (piece.getType() == ChessPiece::KING || piece.getType() == ChessPiece::NONE) {
		return false;
	}

	gf::Vector2i caseProvocateEchec(-1);
	if (p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec;
	}

	Piece empty = Piece(ChessColor::NONE, ChessPiece::NONE);
	Piece copy = piece;
	piece = empty;

	bool res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if (res) {
		piece = copy;
	}else {
		p.bin.push_back(copy);
	}

	return !res;
}

bool DesintegrationIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel Désintégration jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	bool has_notking = false;
	for (auto c : p.state) {
		if (c.piece.getType() != ChessPiece::KING && c.piece.getType() != ChessPiece::NONE && c.piece.getColor() == p.turnTo) {
			has_notking = true;
			break;
		}
	}

	return has_notking;
}

bool AmourCourtois (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel AmourCourtois execute\n");
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])){
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;


	if(!checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KNIGHT), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
		return false;
	}

	Piece pieceVide = piece1;
	gf::Vector2i coordVide = tabVector[0];

	if(piece1.getType()==ChessPiece::KNIGHT) {
		pieceVide = piece2;
		coordVide = tabVector[1];
	}

	for(auto &c : p.state) {
		if(c.piece.getType()==ChessPiece::QUEEN && c.piece.getColor()==p.turnTo) {
			gf::Vector2i pos = c.position;
			if(coordVide.x >= pos.x-1 && coordVide.x <= pos.x+1 && coordVide.y >= pos.y-1 && coordVide.y <= pos.y+1) {
				std::swap(piece1, piece2);
				return true;
			}
		}
	}

	return false;
}

bool AmourCourtoisIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel AmourCourtois jouable\n");

	if (f != Phase::AVANT_COUP  || p.playerInEchec) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) && pieceExist(p, ChessPiece::QUEEN, p.turnTo);
}

bool AvionPrive(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel AvionPrivé execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KING), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool AvionPriveIsPlayable(Plateau& p, Phase f) { 
	gf::Log::info("Appel AvionPrivé jouable\n");

	return !(f != Phase::AVANT_COUP  || p.playerInEchec);
}

bool Apartheid([[maybe_unused]] Plateau& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Apartheid execute\n");
	
	Plateau copie = p;

	for(auto &c : copie.state) {
		int x = c.position.x;
		int y = c.position.y;
		ChessColor colorCase = ChessColor::WHITE;
		
		if (y % 2 == 0) {
			if (x % 2 != 0) { // case foncée
				colorCase = ChessColor::BLACK;
			}
		} else {
			if (x % 2 == 0) { // case foncée
				colorCase = ChessColor::BLACK;
			}
		}
		
		if(c.piece.getType()==ChessPiece::PAWN && c.piece.getColor() == (!colorCase)) { // bye bye le pion
			copie.bin.push_back(c.piece);
			c.piece = Piece(ChessColor::NONE, ChessPiece::NONE);
		}
	}

	gf::Vector2i caseProvocateEchec(-1);
	if(copie.playerInEchec) {
		caseProvocateEchec = copie.caseProvocateEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
	}

	bool res = copie.isInEchec(copie.turnTo) || copie.isInEchec(!copie.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if(!res) {
		p = copie;
	}

	return !res;
}

bool ApartheidIsPlayable([[maybe_unused]] Plateau& p, Phase f) { 
	gf::Log::info("Appel Apartheid jouable\n");

	return f == Phase::APRES_COUP && (pieceExist(p, ChessPiece::PAWN, p.turnTo) || pieceExist(p, ChessPiece::PAWN, !p.turnTo));
}

bool Box(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Box execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(!p.turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.turnTo, ChessPiece::ROOK))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool BoxIsPlayable(Plateau& p, Phase f) { 
	gf::Log::info("Appel Box jouable\n");

	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, !p.turnTo) && pieceExist(p, ChessPiece::ROOK, !p.turnTo);
}

bool ChangerVosCavaliers(Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel ChangerVosCavaliers execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.turnTo, ChessPiece::KNIGHT))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool ChangerVosCavaliersIsPlayable(Plateau& p, Phase f) { 
	gf::Log::info("Appel ChangerVosCavaliers jouable\n");

	if(f!= Phase::APRES_COUP){
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) && pieceExist(p, ChessPiece::KNIGHT, !p.turnTo);
}

bool CavalierSuicide (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Cavalier Suicide execute\n");
	
	if(tabVector.size() != 1 || !inBoard(tabVector[0])){
		return false;
	}

	Plateau plateauCopie = p;
	Piece &piece = plateauCopie.state[tabVector[0].y * 8 + tabVector[0].x].piece;

	if(piece.getType() != ChessPiece::KNIGHT || piece.getColor() != plateauCopie.turnTo) {
		return false;
	}
	
	std::vector<gf::Vector2i> allMoves = piece.getMoveKnight(tabVector[0]);
	
	plateauCopie.bin.push_back(piece);
	piece = Piece(ChessColor::NONE, ChessPiece::NONE);
	
	for(auto coord : allMoves) {	
		if(plateauCopie.state[coord.y * 8 + coord.x].piece.getColor() == !(p.turnTo)) {
			Piece cp = plateauCopie.state[coord.y * 8 + coord.x].piece;
			plateauCopie.state[coord.y * 8 + coord.x].piece = Piece(ChessColor::NONE, ChessPiece::NONE);
			plateauCopie.bin.push_back(cp);
		}
	}

	gf::Vector2i caseProvocateEchec(-1);
	if (plateauCopie.playerInEchec) {
		caseProvocateEchec = plateauCopie.caseProvocateEchec;
	}

	bool res = plateauCopie.isInEchec(plateauCopie.turnTo) || plateauCopie.isInEchec(!plateauCopie.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if (!res) {
		p = plateauCopie;
	}

	return !res;
}

bool CavalierSuicideIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel CavalierSuicide jouable\n");

	if (f != Phase::AVANT_COUP  || p.playerInEchec) {
		return false;
	}

	if(!pieceExist(p, ChessPiece::KNIGHT, p.turnTo)) {
		return false;
	}

	std::size_t nbCasesFull = 0;

	for(auto &c : p.state) {
		nbCasesFull = 0;
		if(c.piece.getType()==ChessPiece::KNIGHT && c.piece.getColor()==p.turnTo) {
			std::vector<gf::Vector2i> allMoves = c.piece.getMoveKnight(c.position);
			for(auto &move : allMoves) {
				if(p.state[move.y *8 + move.x].piece.getType()!=ChessPiece::NONE && p.state[move.y *8 + move.x].piece.getColor() == (!p.turnTo)) {
					nbCasesFull++;
				}
			}
			if(nbCasesFull > 1) {
				return true;
			}
		}
	}
	
	return false;
}

bool RebeloteEtDixDeDer([[maybe_unused]] Plateau& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel RebeloteEtDixDeFer execute\n");
	
	return true;
}

bool RebeloteEtDixDeDerIsPlayable([[maybe_unused]] Plateau& p, Phase f) { 
	gf::Log::info("Appel RebeloteEtDixDeFer jouable\n");

	return f== Phase::APRES_COUP || f== Phase::AVANT_COUP;
}

bool OhDarling (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel Oh Darling execute\n");
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])){
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(!checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KING), piece2, Piece(ChessColor::NONE, ChessPiece::NONE)) && !checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::QUEEN), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
		return false;
	}

	Piece pieceVide = piece1;
	gf::Vector2i coordVide = tabVector[0];

	if(piece1.getType()==ChessPiece::QUEEN || piece1.getType()==ChessPiece::KING) {
		pieceVide = piece2;
		coordVide = tabVector[1];
	}

	ChessPiece aTrouver = ((piece1.getType() == ChessPiece::NONE) ? piece2.getType() : piece1.getType());
	aTrouver = ((aTrouver == ChessPiece::QUEEN ? ChessPiece::KING : ChessPiece::QUEEN));

	for(auto &c : p.state) {
		if(c.piece.getType() == aTrouver && c.piece.getColor()==p.turnTo) {
			gf::Vector2i pos = c.position;
			if(coordVide.x >= pos.x-1 && coordVide.x <= pos.x+1 && coordVide.y >= pos.y-1 && coordVide.y <= pos.y+1) {
				std::swap(piece1, piece2);
				return true;
			}
		}
	}

	return false;
}

bool OhDarlingIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel Oh Darling jouable\n");

	if (f != Phase::AVANT_COUP  || p.playerInEchec) {
		return false;
	}

	return pieceExist(p, ChessPiece::QUEEN, p.turnTo);
}

bool Neutralite (Plateau& p, std::vector<gf::Vector2i> tabVector){

	gf::Log::info("Appel Neutralite execute\n");
	if(tabVector.size() != 1 || !inBoard(tabVector[0])){
		return false;
	}

	Piece &piece = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	if(piece.getType() == ChessPiece::KING || piece.getType() == ChessPiece::QUEEN || piece.getColor() == p.turnTo){
		return false;
	}

	gf::Vector2i caseProvocateEchec(-1);
	if (p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec;
	}

	Piece pieceChoose(ChessColor::GREY,piece.getType());

	std::swap(piece, pieceChoose);

	bool res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if (res) {
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		std::swap(piece, pieceChoose);
	}

	return !res;
}

bool NeutraliteIsPlayable ([[maybe_unused]] Plateau& p, Phase f) {
	gf::Log::info("Appel Neutralite jouable\n");

	if (f != Phase::APRES_COUP) {
		return false;
	}

	return true;
}

bool Ecurie (Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel Ecurie execute\n");
	
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(p.turnTo, ChessPiece::KNIGHT), piece2, Piece(p.turnTo, ChessPiece::ROOK))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool EcurieIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel Ecurie jouable\n");

	if (f != Phase::APRES_COUP ) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) && pieceExist(p, ChessPiece::ROOK, p.turnTo);
}

bool EncephalopathieSongiformeEquine (Plateau& p, std::vector<gf::Vector2i> tabVector){
	gf::Log::info("Appel EncephalopathieSongiformeEquine execute\n");
	
	if(tabVector.size() != 1 || !inBoard(tabVector[0])) {
		return false;
	}

	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;

	if(piece1.getType()!=ChessPiece::KNIGHT) {
		return false;
	}	

	piece1 = Piece(piece1.getColor(), ChessPiece::BISHOP);

	gf::Vector2i caseProvocateEchec(-1);
	if (p.playerInEchec) {
		caseProvocateEchec = p.caseProvocateEchec;
	}

	bool res = p.isInEchec(p.turnTo) || p.isInEchec(!p.turnTo, gf::Vector2i(-1), caseProvocateEchec);
	if (res) {
		gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
		piece1 = Piece(piece1.getColor(), ChessPiece::KNIGHT);
		return false;
	}

	return !res;
}

bool EncephalopathieSongiformeEquineIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel EncephalopathieSongiformeEquine jouable\n");

	if (f != Phase::APRES_COUP ) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, p.turnTo) || pieceExist(p, ChessPiece::KNIGHT, !p.turnTo);
}

bool CrazyHorse (Plateau& p, std::vector<gf::Vector2i> tabVector) {
	gf::Log::info("Appel CrazyHorse execute\n");
	if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
		return false;
	}
	
	Piece &piece1 = p.state[tabVector[0].y * 8 + tabVector[0].x].piece;
	Piece &piece2 = p.state[tabVector[1].y * 8 + tabVector[1].x].piece;

	if(checkGoodChoose(p, piece1, Piece(!p.turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.turnTo, ChessPiece::BISHOP))) {
		std::swap(piece1, piece2);
		return true;
	}

	return false;
}

bool CrazyHorseIsPlayable (Plateau& p, Phase f) {
	gf::Log::info("Appel CrazyHorse jouable\n");
	if (f != Phase::APRES_COUP) {
		return false;
	}

	return pieceExist(p, ChessPiece::KNIGHT, !p.turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.turnTo);
}