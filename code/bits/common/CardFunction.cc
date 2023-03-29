#include "CardFunction.h"

#include <cassert>

#include <gf/Log.h>

namespace tsl {

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

    bool binNotChange(Board& p){

        int len = p.m_allPositions.size();
        
        if (len - 2 < 0) {
            return false;
        }

        std::string before = p.m_allPositions[len - 2];
        std::string now = p.m_allPositions.back();
        std::string not_piece = "1234567890/";


        if (len >= 3){
            int index = (p.m_turnTo == ChessColor::WHITE ? 0:7 );
            char c = (p.m_turnTo == ChessColor::WHITE ? 'P':'p' );
            auto tabS = explode(before,'/');
            if(tabS[index].find(c,0) != std::string::npos){
                before = p.m_allPositions[len - 3];
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

    bool isInEchecAfterCardGreen(Board &p , std::function<bool(Board&, std::vector<gf::Vector2i> tabVector)> execute) {
        Board pp = p;
        
        assert(pp.getFen() == p.getFen());

        execute(pp, {});

        bool ret = true;
        gf::Vector2i m_caseProvokeEchec(-1);
        if(p.m_playerInEchec) {
            m_caseProvokeEchec = p.m_caseProvokeEchec; //obtenir la dernier case qui met met en cehc l'afversaire forcement par un coup normal
        }

        // chercher si une autre case case met en echec l'adversaire apres l'activation de la carte
        if(p.m_turnTo == ChessColor::WHITE) {
            ret = pp.isInEchec(ChessColor::WHITE) || pp.isInEchec(ChessColor::BLACK, gf::Vector2i(-1), m_caseProvokeEchec);
        }else {
            ret = pp.isInEchec(ChessColor::WHITE, gf::Vector2i(-1), m_caseProvokeEchec) || pp.isInEchec(ChessColor::BLACK);
        }

        return ret ;
    }

    //card
    bool NoCard ([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector) { 
        return false;
    }
    bool NoCardPlayable ([[maybe_unused]] Board& p, [[maybe_unused]] Phase f) { 
        return false;
    }

    bool Princess (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Apelle Princesse execute\n");
        if(tabVector.size() != 0){return false;}
        gf::Vector2i pos = (p.m_turnTo  == ChessColor::WHITE ? gf::Vector2i(3,7): gf::Vector2i(3,0)); 
        Case &c = p.m_state[pos.y * 8 + pos.x];
        c.m_piece = Piece(p.m_turnTo, ChessPiece::PRINCESS);

        return true;
    }

    bool PrincessIsPlayable (Board& p, Phase f){
        gf::Log::info("Appel Princesse jouable\n");
        if (f != Phase::APRES_COUP) {
            return false;
        }
        
        if (isInEchecAfterCardGreen(p, Princess)){
            gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
            return false;
        }

        char to_find = (p.m_turnTo  == ChessColor::WHITE ? 'Q' : 'q');
        gf::Vector2i pos = (p.m_turnTo  == ChessColor::WHITE ? gf::Vector2i(3,7): gf::Vector2i(3,0)); 
        std::string lastBoard = p.getFen();

        size_t res = lastBoard.find(to_find); 

        if (res == std::string::npos) {
            if (p.m_state[pos.y * 8 + pos.x].m_piece.getType() == ChessPiece::NONE){
                return true;
            }
        } 
        return false;
    }

    bool BombeAtomique (Board& p, std::vector<gf::Vector2i> tabVector) {
        gf::Log::info("Appel Bombe atomique execute\n");
        if(tabVector.size() != 0){return false;}
        std::vector<gf::Vector2i> targets;

        assert(p.m_lastCoup.size()>1);
        gf::Vector2i pos = p.m_lastCoup.back();

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (inBoard({pos.x+i, pos.y+j})) {
                    //gf::Log::info("la : i = %i et j = %i\n", i, j);
                    targets.push_back(gf::Vector2i(pos.x+i, pos.y+j));
                }
            }
        }

        for (auto& v : targets) {
            if(p.m_state[v.y * 8 + v.x].m_piece.getType() != ChessPiece::KING && p.m_state[v.y * 8 + v.x].m_piece.getType() != ChessPiece::NONE) {
                Piece piece = p.m_state[v.y * 8 + v.x].m_piece;
                p.m_bin.push_back(piece);
                p.m_state[v.y * 8 + v.x].m_piece = Piece(ChessColor::NONE, ChessPiece::NONE);
            }
        }

        return true;
    }

    bool BombeAtomiqueIsPlayable (Board& p, Phase f){
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

    bool Vampirisme (Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Vampirisme execute\n");
        if(tabVector.size() != 0){return false;}
        gf::Vector2i pos = p.m_lastCoup.back();
        p.m_state[pos.y * 8 + pos.x].m_piece = Piece(p.m_turnTo, p.m_bin.back().getType());

        return true;
    }

    bool VampirismeIsPlayable (Board& p, Phase f){
        gf::Log::info("Appel Vampirisme jouable\n");

        if (f != Phase::APRES_COUP || binNotChange(p)) {
            return false;
        }
        
        if (isInEchecAfterCardGreen(p, Vampirisme)){
            gf::Log::info("Cette carte met en Echec l'un des deux rois - donc invalide\n");
            return false;
        }

        gf::Vector2i pos = p.m_lastCoup.back();
        
        return p.m_state[pos.y * 8 + pos.x].m_piece.getType() != ChessPiece::KING;
    }

    bool VisitesOfficielles (Board& p, std::vector<gf::Vector2i> tabVector){
        gf::Log::info("Appel Visites Officielles execute\n");
        if(tabVector.size() != 0){return false;}
        gf::Vector2i roiB(-1);
        gf::Vector2i roiN(-1);

        for(auto &caseEchiquier: p.m_state){
            if(caseEchiquier.m_piece.getType()==ChessPiece::KING && caseEchiquier.m_piece.getColor() == ChessColor::WHITE){
                roiB = caseEchiquier.m_position;
            }
            if(caseEchiquier.m_piece.getType()==ChessPiece::KING && caseEchiquier.m_piece.getColor() == ChessColor::BLACK){
                roiN = caseEchiquier.m_position;
            }
        }

        std::swap(p.m_state[roiB.y*8+roiB.x].m_piece, p.m_state[roiN.y*8+roiN.x].m_piece);

        return true;
    }

    bool VisitesOfficiellesIsPlayable (Board& p, Phase f){
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

}