#ifndef TSL_CARD_FUNC_CHOOSE_CASES_H
#define TSL_CARD_FUNC_CHOOSE_CASES_H

#include "Board.h"
#include <functional>

namespace tsl {

    //tools
    bool pieceExist(Board &p,ChessPiece type, ChessColor color);
    bool checkGoodCase(Board &p, gf::Vector2i coord, ChessPiece type, ChessColor color);
    bool checkGoodChoose(Board &p, Piece &pieceChooseOne, Piece refA , Piece &pieceChooseTwo, Piece refB);

    //cards
    bool Chameau (Board& p, std::vector<gf::Vector2i> tabVector);
    bool QuatreCoin (Board& p, std::vector<gf::Vector2i> tabVector);
    bool ChevalFou (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Asile (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Urbanisme (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Schizophrenie (Board& p, std::vector<gf::Vector2i> tabVector);
    bool TourFada (Board& p, std::vector<gf::Vector2i> tabVector);
    bool BonnesCopines (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Desintegration (Board& p, std::vector<gf::Vector2i> tabVector);
    bool AmourCourtois (Board& p, std::vector<gf::Vector2i> tabVector);
    bool AvionPrive (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Apartheid ([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector);
    bool Box (Board& p, std::vector<gf::Vector2i> tabVector);
    bool ChangerVosCavaliers (Board& p, std::vector<gf::Vector2i> tabVector);
    bool CavalierSuicide (Board& p, std::vector<gf::Vector2i> tabVector);
    // inclue mais a verifier
    bool RebeloteEtDixDeDer ([[maybe_unused]] Board& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector);
    bool OhDarling (Board& p, std::vector<gf::Vector2i> tabVector); 
    // nest pas encore inclut
    bool Neutralite (Board& p, std::vector<gf::Vector2i> tabVector); 
    bool Tir (Board& p, std::vector<gf::Vector2i> tabVector);
    bool Ecurie (Board& p, std::vector<gf::Vector2i> tabVector); 
    bool EncephalopathieSongiformeEquine (Board& p, std::vector<gf::Vector2i> tabVector); 
    bool CrazyHorse (Board& p, std::vector<gf::Vector2i> tabVector); 
    bool Frayeur (Board& p, std::vector<gf::Vector2i> tabVector);
    bool GrosseDeprime (Board& p, std::vector<gf::Vector2i> tabVector); 

    bool ChameauIsPlayable (Board& p, Phase f);
    bool QuatreCoinIsPlayable (Board& p, Phase f);
    bool ChevalFouIsPlayable (Board& p, Phase f);
    bool AsileIsPlayable (Board& p, Phase f);
    bool UrbanismeIsPlayable (Board& p, Phase f);
    bool SchizophrenieIsPlayable (Board& p, Phase f);
    bool TourFadaIsPlayable (Board& p, Phase f);
    bool BonnesCopinesIsPlayable (Board& p, Phase f);
    bool DesintegrationIsPlayable (Board& p, Phase f);
    bool AmourCourtoisIsPlayable (Board& p, Phase f);
    bool AvionPriveIsPlayable (Board& p, Phase f);
    bool ApartheidIsPlayable ([[maybe_unused]] Board& p, Phase f);
    bool BoxIsPlayable (Board& p, Phase f);
    bool ChangerVosCavaliersIsPlayable (Board& p, Phase f);
    bool CavalierSuicideIsPlayable (Board& p, Phase f);
    bool RebeloteEtDixDeDerIsPlayable ([[maybe_unused]] Board& p, Phase f);
    bool NeutraliteIsPlayable ([[maybe_unused]] Board& p, Phase f);
    bool OhDarlingIsPlayable (Board& p, Phase f);
    bool TirIsPlayable ([[maybe_unused]] Board& p, Phase f);
    bool EcurieIsPlayable (Board& p, Phase f);
    bool EncephalopathieSongiformeEquineIsPlayable (Board& p, Phase f);
    bool CrazyHorseIsPlayable (Board& p, Phase f);
    bool FrayeurIsPlayable (Board& p, Phase f);
    bool GrosseDeprimeIsPlayable (Board& p, Phase f);
}

#endif // TSL_CARD_FUNC_CHOOSE_CASES_H