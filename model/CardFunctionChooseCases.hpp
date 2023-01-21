#ifndef CARD_FUNC_CHOOSE_CASES_H
#define CARD_FUNC_CHOOSE_CASES_H

#include "Plateau.hpp"

//tools
bool isPossibleSwapPieces(Plateau &p,ChessPiece a, ChessColor x, ChessPiece b, ChessColor y);
bool checkGoodChoose(ChessPiece typeOne,ChessPiece pieceChooseOne, ChessPiece typeTwo,ChessPiece pieceChooseTwo);

//cards
void Chameau (Plateau& p, gf::Vector2i s);
void QuatreCoin (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void ChevalFou (Plateau& p, gf::Vector2i s, gf::Vector2i e);
void Asile (Plateau& p, gf::Vector2i s, gf::Vector2i e);
 
bool ChameauIsPlayable (Plateau& p, Phase f, gf::Vector2i c);
bool QuatreCoinIsPlayable (Plateau& p, Phase f);
bool ChevalFouIsPlayable (Plateau& p, Phase f);
bool AsileIsPlayable (Plateau& p, Phase f);

#endif // CARD_FUNC_CHOOSE_CASES_H
