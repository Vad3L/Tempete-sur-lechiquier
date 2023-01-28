#ifndef CARD_FUNC_CHOOSE_CASES_H
#define CARD_FUNC_CHOOSE_CASES_H

#include "Plateau.hpp"

//tools
bool pieceExist(Plateau &p,ChessPiece type, ChessColor color);
bool checkGoodCase(Plateau &p, gf::Vector2i coord, ChessPiece type, ChessColor color);
bool checkGoodChoose(Plateau &p, Piece &pieceChooseOne, Piece refA , Piece &pieceChooseTwo, Piece refB);

//cards
bool Chameau (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool QuatreCoin (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool ChevalFou (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Asile (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Urbanisme (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Schizophrenie (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool TourFada (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool BonnesCopines (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Desintegration (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool AmourCourtois (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool AvionPrive (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool Apartheid ([[maybe_unused]] Plateau& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector);
bool Box (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool ChangerVosCavaliers (Plateau& p, std::vector<gf::Vector2i> tabVector);
bool CavalierSuicide (Plateau& p, std::vector<gf::Vector2i> tabVector);
// inclue mais a verifier
bool RebeloteEtDixDeDer ([[maybe_unused]] Plateau& p, [[maybe_unused]] std::vector<gf::Vector2i> tabVector);
bool OhDarling (Plateau& p, std::vector<gf::Vector2i> tabVector); 
// nest pas encore inclut
bool Neutralite (Plateau& p, std::vector<gf::Vector2i> tabVector); 
bool Ecurie (Plateau& p, std::vector<gf::Vector2i> tabVector); 
bool EncephalopathieSongiformeEquine (Plateau& p, std::vector<gf::Vector2i> tabVector); 
bool CrazyHorse (Plateau& p, std::vector<gf::Vector2i> tabVector); 
bool GrosseDeprime (Plateau& p, std::vector<gf::Vector2i> tabVector); 

bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);
bool AsileIsPlayable (Plateau& p, Phase f);
bool UrbanismeIsPlayable (Plateau& p, Phase f);
bool SchizophrenieIsPlayable (Plateau& p, Phase f);
bool TourFadaIsPlayable (Plateau& p, Phase f);
bool BonnesCopinesIsPlayable (Plateau& p, Phase f);
bool DesintegrationIsPlayable (Plateau& p, Phase f);
bool AmourCourtoisIsPlayable (Plateau& p, Phase f);
bool AvionPriveIsPlayable (Plateau& p, Phase f);
bool ApartheidIsPlayable ([[maybe_unused]] Plateau& p, Phase f);
bool BoxIsPlayable (Plateau& p, Phase f);
bool ChangerVosCavaliersIsPlayable (Plateau& p, Phase f);
bool CavalierSuicideIsPlayable (Plateau& p, Phase f);
bool RebeloteEtDixDeDerIsPlayable ([[maybe_unused]] Plateau& p, Phase f);
bool NeutraliteIsPlayable ([[maybe_unused]] Plateau& p, Phase f);
bool OhDarlingIsPlayable (Plateau& p, Phase f);
bool EcurieIsPlayable (Plateau& p, Phase f);
bool EncephalopathieSongiformeEquineIsPlayable (Plateau& p, Phase f);
bool CrazyHorseIsPlayable (Plateau& p, Phase f);
bool GrosseDeprimeIsPlayable (Plateau& p, Phase f);

#endif // CARD_FUNC_CHOOSE_CASES_H
