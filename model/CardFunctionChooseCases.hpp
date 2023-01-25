#ifndef CARD_FUNC_CHOOSE_CASES_H
#define CARD_FUNC_CHOOSE_CASES_H

#include "Plateau.hpp"

//tools
bool pieceExist(Plateau &p,ChessPiece type, ChessColor color);
bool checkGoodCase(Plateau &p, gf::Vector2i coord, ChessPiece type, ChessColor color);
bool checkGoodChoose(Plateau &p, Piece &pieceChooseOne, Piece refA , Piece &pieceChooseTwo, Piece refB);

//cards
bool Chameau (Plateau& p, gf::Vector2i s,gf::Vector2i e);
bool QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e);
bool Asile (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);
bool AsileIsPlayable (Plateau& p, Phase f);

#endif // CARD_FUNC_CHOOSE_CASES_H
