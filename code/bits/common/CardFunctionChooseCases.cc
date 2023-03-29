#include "CardFunctionChooseCases.h"

#include <gf/Log.h>

namespace tsl {
        
    bool pieceExist(Board &p,ChessPiece type, ChessColor color) {
        for(auto caseEchiquier: p.m_state){
            if(caseEchiquier.m_piece.getType() == type && caseEchiquier.m_piece.getColor() == color) {
                return true;
            }
        }
        return false;
    }

    bool checkGoodCase(Board &p, gf::Vector2i coord, ChessPiece type, ChessColor color) {
        Piece &piece = p.m_state[coord.y * 8 + coord.x].m_piece;
        return piece.getType() == type &&  piece.getColor() == color;
    }

    bool checkGoodChoose(Board &p, Piece &pieceChooseOne, Piece refA , Piece &pieceChooseTwo, Piece refB) {
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
        if(p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
        }

        // chercher si une autre case case met en echec l'adversaire apres l'activation de la carte
        /*if(p.m_turnTo == ChessColor::WHITE) {
            res = p.isInEchec(ChessColor::WHITE) || p.isInEchec(ChessColor::BLACK, gf::Vector2i(-1), caseProvocateEchec);
        }else {
            res = p.isInEchec(ChessColor::WHITE, gf::Vector2i(-1), caseProvocateEchec) || p.isInEchec(ChessColor::BLACK);
        }*/
        res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);

        std::swap(pieceChooseTwo, pieceChooseOne);
        return !res;
    }

    bool ChevalFou (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel ChevalFou execute\n");
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(p.m_turnTo, ChessPiece::BISHOP))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool ChevalFouIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel ChevalFou jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo) && pieceExist(p, ChessPiece::BISHOP, p.m_turnTo);
    }


    bool Chameau (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Chameau execute\n");
        if(tabVector.size() != 1 || !inBoard(tabVector[0])) {
            return false;
        }
        Piece& piece=p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;

        if (piece.getType() != ChessPiece::KNIGHT) {
            return false;
        }
        ChessColor playerColor = piece.getColor();

        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        piece = Piece(playerColor, ChessPiece::CAMEL);

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            piece = Piece(playerColor, ChessPiece::KNIGHT);
        }
        return !res;
    }

    bool ChameauIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel chameau jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo);
    }


    bool QuatreCoin (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Quatre coin execute\n");
        if(tabVector.size() != 1 || !inBoard(tabVector[0])){
            return false;
        }
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;

        if(piece1.getType() == ChessPiece::NONE || piece1.getColor() != p.m_turnTo) {
            return false;
        }

        gf::Vector2i coordLibre(-1);
        std::vector<gf::Vector2i> coins = { gf::Vector2i(0, 0),
                                            gf::Vector2i(0, 7),
                                            gf::Vector2i(7, 0),
                                            gf::Vector2i(7, 7) };

        for (auto c : coins) {
            if (p.m_state[c.y * 8 + c.x].m_piece.getType() == ChessPiece::NONE) {
                coordLibre = c;
                break;
            }
        }
        
        if(!inBoard(coordLibre)){return false;}

        Piece &piece2 = p.m_state[coordLibre.y * 8 + coordLibre.x].m_piece;

        std::swap(piece1, piece2);

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo);
        if (res) {
            std::swap(piece1, piece2);
        }

        return !res;
    }

    bool QuatreCoinIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Quatre coin jouable\n");

        if (f != Phase::AVANT_COUP  || p.m_playerInEchec) {
            return false;
        }

        int occupied = 0;

        std::vector<gf::Vector2i> coins = { 	gf::Vector2i(0, 0),
                                                gf::Vector2i(0, 7),
                                                gf::Vector2i(7, 0),
                                                gf::Vector2i(7, 7) };

        for (auto c : coins) {
            if (p.m_state[c.y * 8 + c.x].m_piece.getType() != ChessPiece::NONE) {
                occupied++;
            }
        }

        return occupied == 3;
    }

    bool Asile(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Asile execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::ROOK), piece2, Piece(p.m_turnTo, ChessPiece::BISHOP))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }


    bool AsileIsPlayable(Board& p, Phase f){ 
        gf::Log::info("Appel Asile jouable\n");
        if(f!= Phase::APRES_COUP){
            return false;
        }

        return pieceExist(p, ChessPiece::ROOK, p.m_turnTo) && pieceExist(p, ChessPiece::BISHOP, p.m_turnTo);
    }

    bool TourFada (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Tour Fada execute\n");
        if (tabVector.size() != 2) {
            return false;
        }

        if (!inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }

        Piece& piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece& piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if (checkGoodChoose(p, piece1, Piece(!p.m_turnTo, ChessPiece::ROOK), piece2, Piece(!p.m_turnTo, ChessPiece::BISHOP))) {
            std::swap(piece1, piece2);
            return true;
        }
        return false;
    }

    bool TourFadaIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Tour Fada jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::ROOK, !p.m_turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.m_turnTo);
    }

    bool Urbanisme(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Urbansime execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::ROOK), piece2, Piece(!p.m_turnTo, ChessPiece::ROOK))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool UrbanismeIsPlayable(Board& p, Phase f) { 
        gf::Log::info("Appel Urbanisme jouable\n");

        if(f!= Phase::APRES_COUP){
            return false;
        }

        return pieceExist(p, ChessPiece::ROOK, p.m_turnTo) && pieceExist(p, ChessPiece::ROOK, !p.m_turnTo);
    }

    bool Schizophrenie(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Schizophrenie execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::BISHOP), piece2, Piece(!p.m_turnTo, ChessPiece::BISHOP))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool SchizophrenieIsPlayable(Board& p, Phase f) { 
        gf::Log::info("Appel Schizophrenie jouable\n");

        if(f!= Phase::APRES_COUP){
            return false;
        }

        return pieceExist(p, ChessPiece::BISHOP, p.m_turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.m_turnTo);
    }

    bool BonnesCopines(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel BonnesCopines execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::QUEEN), piece2, Piece(!p.m_turnTo, ChessPiece::QUEEN))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }


    bool BonnesCopinesIsPlayable(Board& p, Phase f){ 
        gf::Log::info("Appel BonnesCopines jouable\n");
        if(f!= Phase::AVANT_COUP || p.m_playerInEchec){
            return false;
        }

        return pieceExist(p, ChessPiece::QUEEN, p.m_turnTo) && pieceExist(p, ChessPiece::QUEEN, !p.m_turnTo);
    }

    bool Desintegration (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Désintégration execute\n");
        if (tabVector.size() != 1) {
            return false;
        }

        if (tabVector.size() != 1 || !inBoard(tabVector[0])) {
            return false;
        }

        Piece& piece = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        if (piece.getColor() != p.m_turnTo) {
            return false;
        }
        if (piece.getType() == ChessPiece::KING || piece.getType() == ChessPiece::NONE) {
            return false;
        }

        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        Piece empty = Piece(ChessColor::NONE, ChessPiece::NONE);
        Piece copy = piece;
        piece = empty;

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            piece = copy;
        }else {
            p.m_bin.push_back(copy);
        }

        return !res;
    }

    bool DesintegrationIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Désintégration jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        bool has_notking = false;
        for (auto c : p.m_state) {
            if (c.m_piece.getType() != ChessPiece::KING && c.m_piece.getType() != ChessPiece::NONE && c.m_piece.getColor() == p.m_turnTo) {
                has_notking = true;
                break;
            }
        }

        return has_notking;
    }

    bool AmourCourtois (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel AmourCourtois execute\n");
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])){
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;


        if(!checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
            return false;
        }

        Piece pieceVide = piece1;
        gf::Vector2i coordVide = tabVector[0];

        if(piece1.getType()==ChessPiece::KNIGHT) {
            pieceVide = piece2;
            coordVide = tabVector[1];
        }

        for(auto &c : p.m_state) {
            if(c.m_piece.getType()==ChessPiece::QUEEN && c.m_piece.getColor()==p.m_turnTo) {
                gf::Vector2i pos = c.m_position;
                if(coordVide.x >= pos.x-1 && coordVide.x <= pos.x+1 && coordVide.y >= pos.y-1 && coordVide.y <= pos.y+1) {
                    std::swap(piece1, piece2);
                    return true;
                }
            }
        }

        return false;
    }

    bool AmourCourtoisIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel AmourCourtois jouable\n");

        if (f != Phase::AVANT_COUP  || p.m_playerInEchec) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo) && pieceExist(p, ChessPiece::QUEEN, p.m_turnTo);
    }

    bool AvionPrive(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel AvionPrivé execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KING), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool AvionPriveIsPlayable(Board& p, Phase f) { 
        gf::Log::info("Appel AvionPrivé jouable\n");

        return !(f != Phase::AVANT_COUP  || p.m_playerInEchec);
    }

    bool Apartheid([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Apartheid execute\n");
        
        Board copie = p;

        for(auto &c : copie.m_state) {
            int x = c.m_position.x;
            int y = c.m_position.y;
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
            
            if(c.m_piece.getType()==ChessPiece::PAWN && c.m_piece.getColor() == (!colorCase)) { // bye bye le pion
                copie.m_bin.push_back(c.m_piece);
                c.m_piece = Piece(ChessColor::NONE, ChessPiece::NONE);
            }
        }

        gf::Vector2i caseProvocateEchec(-1);
        if(copie.m_playerInEchec) {
            caseProvocateEchec = copie.m_caseProvokeEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
        }

        bool res = copie.isInEchec(copie.m_turnTo) || copie.isInEchec(!copie.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if(!res) {
            p = copie;
        }

        return !res;
    }

    bool ApartheidIsPlayable([[maybe_unused]] Board& p, Phase f) { 
        gf::Log::info("Appel Apartheid jouable\n");

        return f == Phase::APRES_COUP && (pieceExist(p, ChessPiece::PAWN, p.m_turnTo) || pieceExist(p, ChessPiece::PAWN, !p.m_turnTo));
    }

    bool Box(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Box execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(!p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.m_turnTo, ChessPiece::ROOK))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool BoxIsPlayable(Board& p, Phase f) { 
        gf::Log::info("Appel Box jouable\n");

        if(f!= Phase::APRES_COUP){
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, !p.m_turnTo) && pieceExist(p, ChessPiece::ROOK, !p.m_turnTo);
    }

    bool ChangerVosCavaliers(Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel ChangerVosCavaliers execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.m_turnTo, ChessPiece::KNIGHT))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool ChangerVosCavaliersIsPlayable(Board& p, Phase f) { 
        gf::Log::info("Appel ChangerVosCavaliers jouable\n");

        if(f!= Phase::APRES_COUP){
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo) && pieceExist(p, ChessPiece::KNIGHT, !p.m_turnTo);
    }

    bool CavalierSuicide (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Cavalier Suicide execute\n");
        
        if(tabVector.size() != 1 || !inBoard(tabVector[0])){
            return false;
        }

        Board plateauCopie = p;
        Piece &piece = plateauCopie.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;

        if(piece.getType() != ChessPiece::KNIGHT || piece.getColor() != plateauCopie.m_turnTo) {
            return false;
        }
        
        std::vector<gf::Vector2i> allMoves = piece.getMoveKnight(tabVector[0]);
        
        plateauCopie.m_bin.push_back(piece);
        piece = Piece(ChessColor::NONE, ChessPiece::NONE);
        
        for(auto coord : allMoves) {	
            if(plateauCopie.m_state[coord.y * 8 + coord.x].m_piece.getColor() == !(p.m_turnTo)) {
                Piece cp = plateauCopie.m_state[coord.y * 8 + coord.x].m_piece;
                plateauCopie.m_state[coord.y * 8 + coord.x].m_piece = Piece(ChessColor::NONE, ChessPiece::NONE);
                plateauCopie.m_bin.push_back(cp);
            }
        }

        gf::Vector2i caseProvocateEchec(-1);
        if (plateauCopie.m_playerInEchec) {
            caseProvocateEchec = plateauCopie.m_caseProvokeEchec;
        }

        bool res = plateauCopie.isInEchec(plateauCopie.m_turnTo) || plateauCopie.isInEchec(!plateauCopie.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (!res) {
            p = plateauCopie;
        }

        return !res;
    }

    bool CavalierSuicideIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel CavalierSuicide jouable\n");

        if (f != Phase::AVANT_COUP  || p.m_playerInEchec) {
            return false;
        }

        if(!pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo)) {
            return false;
        }

        std::size_t nbCasesFull = 0;

        for(auto &c : p.m_state) {
            nbCasesFull = 0;
            if(c.m_piece.getType()==ChessPiece::KNIGHT && c.m_piece.getColor()==p.m_turnTo) {
                std::vector<gf::Vector2i> allMoves = c.m_piece.getMoveKnight(c.m_position);
                for(auto &move : allMoves) {
                    if(p.m_state[move.y *8 + move.x].m_piece.getType()!=ChessPiece::NONE && p.m_state[move.y *8 + move.x].m_piece.getColor() == (!p.m_turnTo)) {
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

    bool RebeloteEtDixDeDer([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel RebeloteEtDixDeFer execute\n");
        
        return true;
    }

    bool RebeloteEtDixDeDerIsPlayable([[maybe_unused]] Board& p, Phase f) { 
        gf::Log::info("Appel RebeloteEtDixDeFer jouable\n");

        return f== Phase::APRES_COUP || f== Phase::AVANT_COUP;
    }

    bool OhDarling (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Oh Darling execute\n");
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])){
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(!checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KING), piece2, Piece(ChessColor::NONE, ChessPiece::NONE)) && !checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::QUEEN), piece2, Piece(ChessColor::NONE, ChessPiece::NONE))) {
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

        for(auto &c : p.m_state) {
            if(c.m_piece.getType() == aTrouver && c.m_piece.getColor()==p.m_turnTo) {
                gf::Vector2i pos = c.m_position;
                if(coordVide.x >= pos.x-1 && coordVide.x <= pos.x+1 && coordVide.y >= pos.y-1 && coordVide.y <= pos.y+1) {
                    std::swap(piece1, piece2);
                    return true;
                }
            }
        }

        return false;
    }

    bool OhDarlingIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Oh Darling jouable\n");

        if (f != Phase::AVANT_COUP  || p.m_playerInEchec) {
            return false;
        }

        return pieceExist(p, ChessPiece::QUEEN, p.m_turnTo);
    }

    bool Neutralite (Board& p, std::vector<gf::Vector2i> tabVector){

        gf::Log::info("Appel Neutralite execute\n");
        if(tabVector.size() != 1 || !inBoard(tabVector[0])){
            return false;
        }

        Piece &piece = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        if(piece.getType() == ChessPiece::KING || piece.getType() == ChessPiece::QUEEN || piece.getColor() == p.m_turnTo){
            return false;
        }

        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        Piece pieceChoose(ChessColor::GREY,piece.getType());

        std::swap(piece, pieceChoose);

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
            std::swap(piece, pieceChoose);
        }

        return !res;
    }

    bool NeutraliteIsPlayable ([[maybe_unused]] Board& p, Phase f) {
        gf::Log::info("Appel Neutralite jouable\n");

        if (f != Phase::APRES_COUP) {
            return false;
        }

        return true;
    }

    bool Ecurie (Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Ecurie execute\n");
        
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(p.m_turnTo, ChessPiece::ROOK))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool EcurieIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Ecurie jouable\n");

        if (f != Phase::APRES_COUP ) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo) && pieceExist(p, ChessPiece::ROOK, p.m_turnTo);
    }

    bool Tir (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Tir execute\n");

        if (tabVector.size() != 2) {
            return false;
        }

        if (!inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }

        size_t us = -1, enemy = -1;
        gf::Vector2i vus(-1), venemy(-1);
        if (p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece.getColor() == p.m_turnTo) {
            us = tabVector[0].y * 8 + tabVector[0].x;
            vus = tabVector[0];
            venemy = tabVector[1];
            enemy = tabVector[1].y * 8 + tabVector[1].x;
        } else {
            us = tabVector[1].y * 8 + tabVector[1].x;
            vus = tabVector[1];
            venemy = tabVector[0];
            enemy = tabVector[0].y * 8 + tabVector[0].x;
        }


        Piece &piece = p.m_state[us].m_piece;
        Piece &target =  p.m_state[enemy].m_piece;
        Piece copy = target;
        Piece empty = Piece(ChessColor::NONE, ChessPiece::NONE);

        if (piece.getColor() == target.getColor()) { return false; }
        if (piece.getType() == ChessPiece::NONE) { return false; }
        if (target.getType() == ChessPiece::NONE) { return false; }
        auto moves = p.filterMoveAuthorized(vus, piece.getMoves(vus));

        bool has_move = false;
        for (auto m : moves) {
            if (m == venemy) { has_move = true; break; }
        }
        if (!has_move) { return false; }

        gf::Vector2i caseEchec(-1);
        
        if (p.m_playerInEchec) {
            caseEchec = p.m_caseProvokeEchec;	
        }

        target = empty;
        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseEchec);

        if (res) {
            target = copy;
        } else {
            p.m_bin.push_back(copy);
        }
        return !res;
    }

    bool TirIsPlayable ([[maybe_unused]] Board& p, Phase f) {
        gf::Log::info("Appel Tir jouable\n");
        return (f == Phase::AVANT_COUP && !p.m_playerInEchec);
    }

    bool EncephalopathieSongiformeEquine (Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel EncephalopathieSongiformeEquine execute\n");
        
        if(tabVector.size() != 1 || !inBoard(tabVector[0])) {
            return false;
        }

        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;

        if(piece1.getType()!=ChessPiece::KNIGHT) {
            return false;
        }	

        piece1 = Piece(piece1.getColor(), ChessPiece::BISHOP);

        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
            piece1 = Piece(piece1.getColor(), ChessPiece::KNIGHT);
        }

        return !res;
    }

    bool EncephalopathieSongiformeEquineIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel EncephalopathieSongiformeEquine jouable\n");

        if (f != Phase::APRES_COUP ) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, p.m_turnTo) || pieceExist(p, ChessPiece::KNIGHT, !p.m_turnTo);
    }

    bool CrazyHorse (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel CrazyHorse execute\n");
        if(tabVector.size() != 2 || !inBoard(tabVector[0]) || !inBoard(tabVector[1])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece &piece2 = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;

        if(checkGoodChoose(p, piece1, Piece(!p.m_turnTo, ChessPiece::KNIGHT), piece2, Piece(!p.m_turnTo, ChessPiece::BISHOP))) {
            std::swap(piece1, piece2);
            return true;
        }

        return false;
    }

    bool CrazyHorseIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel CrazyHorse jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::KNIGHT, !p.m_turnTo) && pieceExist(p, ChessPiece::BISHOP, !p.m_turnTo);
    }

    bool Frayeur (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Frayeur execute\n");
        if (tabVector.size() != 2) { return false; }
        if (!inBoard(tabVector[0]) || !inBoard(tabVector[1])) { return false; }

        Piece& pa = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;
        Piece& pb = p.m_state[tabVector[1].y * 8 + tabVector[1].x].m_piece;
        gf::Vector2i pawn, empty;
        bool correct = false;

        if (pa.getType() == ChessPiece::PAWN && pb.getType() == ChessPiece::NONE) {
            pawn = tabVector[0];
            empty = tabVector[1];
            correct = true;
        } else if (pa.getType() == ChessPiece::NONE && pb.getType() == ChessPiece::PAWN) {
            pawn = tabVector[1];
            empty = tabVector[0];
            correct = true;
        }

        if (!correct) { return false; }
        if (p.m_state[pawn.y * 8 + pawn.x].m_piece.getColor() == p.m_turnTo) { return false; }

        if (pawn.x != empty.x) { return false; }
        if (abs(pawn.y - empty.y) > 2) { return false; }

        bool can = false;
        int col = (!p.m_turnTo == ChessColor::WHITE) ? 1 : -1;
        
        if ((pawn.y + col) == empty.y) { can = true; }
        if ((pawn.y +  2 * col) == empty.y) {
            if (p.m_state[((pawn.y + col) * 8) + pawn.x].m_piece.getType() != ChessPiece::NONE) { return false; }
            can = true;
        }
        
        if (!can) { return false; }
        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        std::swap(pa, pb);

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            std::swap(pa, pb);
        }
        return !res;
    }

    bool FrayeurIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel Frayeur jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::PAWN, !p.m_turnTo);
    }

    bool GrosseDeprime (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel GrosseDeprime execute\n");
        if(tabVector.size() != 1 || !inBoard(tabVector[0])) {
            return false;
        }
        
        Piece &piece1 = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_piece;

        if(piece1.getType()!=ChessPiece::PAWN || piece1.getColor()==p.m_turnTo) {
            return false;
        }

        int x = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_position.x;
        int y = p.m_state[tabVector[0].y * 8 + tabVector[0].x].m_position.y;

        for(int i=-1; i<=1; i++) {
            for(int j=-1; j<=1; j++) {
                if(inBoard({x-i, y-j}) && (i !=0 || j!=0)) {
                    if(!(p.m_state[(y-j)*8 + (x-i)].m_piece.getType()==ChessPiece::NONE && p.m_state[(y-j)*8 + (x-i)].m_piece.getColor()==ChessColor::NONE)) {
                        gf::Log::info("Le pion adverse à un voisin - donc invalide\n");
                        return false;
                    }
                }
            }
        }

        piece1 = Piece(ChessColor::NONE, ChessPiece::NONE);

        gf::Vector2i caseProvocateEchec(-1);
        if (p.m_playerInEchec) {
            caseProvocateEchec = p.m_caseProvokeEchec;
        }

        bool res = p.isInEchec(p.m_turnTo) || p.isInEchec(!p.m_turnTo, gf::Vector2i(-1), caseProvocateEchec);
        if (res) {
            gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
            piece1 = Piece(!p.m_turnTo, ChessPiece::PAWN);
        } else {
            p.m_bin.push_back(Piece(!p.m_turnTo, ChessPiece::PAWN));
        }

        return !res;
    }

    bool GrosseDeprimeIsPlayable (Board& p, Phase f) {
        gf::Log::info("Appel GrosseDeprime jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }

        return pieceExist(p, ChessPiece::PAWN, !p.m_turnTo);
    }

}