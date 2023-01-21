#ifndef CARD_FUNC_CHOOSE_CASES_H
#define CARD_FUNC_CHOOSE_CASES_H

#include "Plateau.hpp"

//tools
bool isPossibleSwapPieces(Plateau &p,ChessPiece a, ChessPiece b, ChessColor y);
bool checkGoodChoose(Plateau &p,ChessPiece typeOne,Piece pieceChooseOne, ChessPiece typeTwo,Piece pieceChooseTwo,ChessColor color);

//cards
void Chameau (Plateau& p, gf::Vector2i s,gf::Vector2i e);
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Asile (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool ChameauIsPlayable (Plateau& p, Phase f);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);
bool AsileIsPlayable (Plateau& p, Phase f);

#endif // CARD_FUNC_CHOOSE_CASES_H
