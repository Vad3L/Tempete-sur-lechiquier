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
 
bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);
bool AsileIsPlayable (Plateau& p, Phase f);
bool UrbanismeIsPlayable (Plateau& p, Phase f);
bool SchizophrenieIsPlayable (Plateau& p, Phase f);
bool TourFadaIsPlayable (Plateau& p, Phase f);
bool BonnesCopinesIsPlayable (Plateau& p, Phase f);
bool DesintegrationIsPlayable (Plateau& p, Phase f);


#endif // CARD_FUNC_CHOOSE_CASES_H
