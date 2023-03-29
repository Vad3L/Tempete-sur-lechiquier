#include "Board.h"

#include <gf/Log.h>
#include <iostream>

namespace tsl {

    Board::Board()
    : m_coordCaseSelected(-1)
    , m_coordTakePassing(-1)
    , m_caseProvokeEchec(-1)
    , m_promotion(false)
    , m_playerInEchec(false) {
        for (int i = 0; i < 8; i++) {
            for (int coordPass = 0; coordPass < 8; coordPass++) {
                m_state.push_back(Case(gf::Vector2i(coordPass, i)));
                if (i == 1 || i == 6) {
                    ChessColor c = (i == 1) ? ChessColor::BLACK : ChessColor::WHITE;
                    m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::PAWN);
                } else if (i == 0 || i == 7) {
                    ChessColor c = (i == 0) ? ChessColor::BLACK : ChessColor::WHITE;
                    switch (coordPass) {
                        case 0:
                        case 7:
                            if(!(i ==0 && coordPass==0))
                            m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::ROOK);
                            break;
                        case 1:
                        case 6:
                            m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::KNIGHT);
                            break;
                        case 2:
                        case 5:
                            m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::BISHOP);
                            break;
                        case 3:
                            m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::QUEEN);
                            break;
                        case 4:
                            m_state[i * 8 + coordPass].m_piece = Piece(c, ChessPiece::KING);
                            break;
                    }
                }
            }
        }

        m_state[5 * 8 + 3].m_piece = Piece(ChessColor::WHITE, ChessPiece::PRINCESS);
        m_state[2 * 8 + 3].m_piece = Piece(ChessColor::BLACK, ChessPiece::PRINCESS);
        m_state[5 * 8 + 4].m_piece = Piece(ChessColor::WHITE, ChessPiece::PRINCE);
        m_state[2 * 8 + 4].m_piece = Piece(ChessColor::BLACK, ChessPiece::PRINCE);
        m_state[5 * 8 + 5].m_piece = Piece(ChessColor::WHITE, ChessPiece::CAMEL);
        m_state[2 * 8 + 5].m_piece = Piece(ChessColor::BLACK, ChessPiece::CAMEL);

        m_state[1 * 8 + 0].m_piece = Piece(ChessColor::WHITE, ChessPiece::PAWN);
        m_state[6 * 8 + 0].m_piece = Piece(ChessColor::BLACK, ChessPiece::PAWN);

        m_state[2 * 8 + 0].m_piece = Piece(ChessColor::GREY, ChessPiece::PAWN);
        m_allPositions.push_back(getFen());
         m_turnTo = ChessColor::WHITE;
        prettyPrint();
    }

    std::string Board::getSymbol(ChessColor c, ChessPiece p) {
        std::vector<std::string> blacks = {
            "♚", "♛", "♝", "♞", "♜", "♟︎", "🨇", "🨏", " "
        };
        std::vector<std::string> whites = {
            "♔", "♕", "♗", "♘", "♖", "♙", "🨆", "🨉", " "
        };
        std::vector<std::string> neutral = {
            "🨀", "🨁", "🨃", "🨄", "🨂", "🨅", "🨈", "🨏", " "
        };

        if (c == ChessColor::WHITE) {
            return whites[(int)p];
        } else if (c == ChessColor::BLACK) {
            return blacks[(int)p];
        } else if (c == ChessColor::GREY) {
            return neutral[(int)p];
        } else {
            return ".";
        }
    }

    void Board::prettyPrint() {
        gf::Log::debug("prettyPrint\n");
        for  (Case &c : this->m_state) {
            std::cout << " " << getSymbol(c.m_piece.getColor(), c.m_piece.getType());
            if(c.m_position.x == 7) {
                std::cout << ("\n");
            }
        }
    }

    bool Board::setMovement(gf::Vector2i v) {
        m_coordTakePassing = gf::Vector2i(-1);

        Piece pSelect = m_state[v.y * 8 + v.x].m_piece;
        ChessColor color =  m_turnTo;

        if( m_coordCaseSelected.y ==-1 &&  m_coordCaseSelected.x == -1) { // aucune première de selectionné
            if( pSelect.getType() != ChessPiece::NONE && pSelect.getColor() == color) { // selectionne case si la piece nous appartient 
                 m_coordCaseSelected = v;
                m_moveAvailable = pSelect.getMoves( m_coordCaseSelected);
                m_moveAvailable = filterMoveAuthorized( m_coordCaseSelected, m_moveAvailable); // à commenter si on veux test la validité d'un coup par le serveur
                return false;
            }
        }else {
            if( m_coordCaseSelected == v) { // deselectionner case
                 m_coordCaseSelected = gf::Vector2i(-1,-1);
                m_moveAvailable.clear();
                return false;
            }
            
            for(auto &coord : m_moveAvailable) {
                if(coord == v) {
                    return true;
                }
            }
            
            if(pSelect.getType() != ChessPiece::NONE && pSelect.getColor() == color) { // selectionne case si la piece nous appartient 
                 m_coordCaseSelected = v;
                m_moveAvailable.clear();
                m_moveAvailable = pSelect.getMoves( m_coordCaseSelected);
                m_moveAvailable = filterMoveAuthorized( m_coordCaseSelected, m_moveAvailable); // à commenter si on veux test la validité d'un coup par le serveur
            }
        }
        return false;
    }


    std::vector<gf::Vector2i> Board::filterMoveAuthorized(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
        assert(coordCaseStart.x >= 0);
        assert(coordCaseStart.x < 8);
        assert(coordCaseStart.y >= 0);
        assert(coordCaseStart.y < 8);

        std::vector<gf::Vector2i> v;
        
        v = filterMoveAuthorized_Tangled(coordCaseStart, mAvailable);
        v = filterMoveAuthorized_Pawn(coordCaseStart, v);
        v = filterMoveAuthorized_Check(coordCaseStart, v);
        v = addMoveBigSmallCastling(coordCaseStart, v);
        
        return v;
    }

    std::vector<gf::Vector2i> Board::filterMoveAuthorized_Tangled(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
        assert(coordCaseStart.x >= 0);
        assert(coordCaseStart.x < 8);
        assert(coordCaseStart.y >= 0);
        assert(coordCaseStart.y < 8);

        std::vector<gf::Vector2i> v;

        Piece piece = m_state[coordCaseStart.y * 8 + coordCaseStart.x].m_piece;
        
        for(auto coordCase : mAvailable) {
            std::vector<gf::Vector2i> casesPass = piece.getCasesPass(coordCaseStart, coordCase);

            bool find = true;
            for(auto coordPass : casesPass) {

                if(m_state[coordPass.y * 8 + coordPass.x].m_piece.getType() != ChessPiece::NONE) {
                
                    // cas collision couleur soi-même
                    bool sameCouleur =  (m_state[coordPass.y * 8 + coordPass.x].m_piece.getColor() == piece.getColor());
                    if(sameCouleur){
                        find=false;
                        break;
                    }

                    // cas collision couleur adverse
                    if(!sameCouleur && !(coordCase.y == coordPass.y && coordCase.x == coordPass.x)){
                        find=false;
                        break;
                    }
                }
            }
            
            if(find){
                v.push_back(coordCase);
            }
        }
        
        //fonction tmp debug
        tmp(v);

        return v;
    }

    std::vector<gf::Vector2i> Board::filterMoveAuthorized_Pawn(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
        assert(coordCaseStart.x >= 0);
        assert(coordCaseStart.x < 8);
        assert(coordCaseStart.y >= 0);
        assert(coordCaseStart.y < 8);
        
        Piece piece = m_state[coordCaseStart.y * 8 + coordCaseStart.x].m_piece;
        
        if(piece.getType() != ChessPiece::PAWN || coordCaseStart.y == 0 || coordCaseStart.y == 7) {
            return mAvailable;
        }
        
        std::vector<gf::Vector2i> v;
            
        for(const auto &coordCase : mAvailable) {
            std::vector<gf::Vector2i> casesPass = piece.getCasesPass(coordCaseStart, coordCase);
            assert(coordCase.x == coordCaseStart.x);

            bool find = true;
            for(auto coordPass : casesPass) {
                Piece piece = m_state[coordPass.y * 8 + coordPass.x].m_piece;
                if(piece.getType() != ChessPiece::NONE) {
                    find = false;
                    break;
                }
            }

            if(find){
                v.push_back(coordCase);
            }
        }

        // one case in front of pawn
        int add = (piece.getColor() == ChessColor::WHITE) ? 1 : -1;
        
        // cas eat in diagonal
        if(coordCaseStart.x-1 >= 0) {
            Piece pL = m_state[(coordCaseStart.y-add) * 8 + coordCaseStart.x-1].m_piece;
            if(pL.getType() != ChessPiece::NONE && pL.getColor() != piece.getColor()) {
                v.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y-add));
            }
        }
        
        if(coordCaseStart.x+1 < 8) {
            Piece pR = m_state[(coordCaseStart.y-add) * 8 + coordCaseStart.x+1].m_piece;
            if(pR.getType() != ChessPiece::NONE && pR.getColor() != piece.getColor()) {
                v.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y-add));
            }
        }		

        ChessColor tab[2] = {ChessColor::WHITE, ChessColor::BLACK};

        // prise en passant
        for(int i = 0 ; i < 2 ; i++) {
            if(piece.getColor() == tab[i] && coordCaseStart.y == 3+i && abs(m_lastCoup.back().y-m_lastCoup[m_lastCoup.size()-2].y) == 2 && m_lastCoup.back().y == 3+i) {
                if(coordCaseStart.x-1 >= 0 && m_lastCoup.back().x == coordCaseStart.x-1) {
                    Piece pL = m_state[coordCaseStart.y * 8 + coordCaseStart.x-1].m_piece;
                    if(pL.getType()==ChessPiece::PAWN && pL.getColor() != piece.getColor()) {
                        v.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y-add));
                        m_coordTakePassing = gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y);
                    }
                }

                if(coordCaseStart.x+1 < 8 && m_lastCoup.back().x == coordCaseStart.x+1) {
                    Piece pR = m_state[coordCaseStart.y * 8 + coordCaseStart.x+1].m_piece;
                    if(pR.getType()==ChessPiece::PAWN && pR.getColor() != piece.getColor()) {
                        v.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y-add));
                        m_coordTakePassing = gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y);
                    }
                }
            }
        }
        
        //fonction tmp debug
        tmp(v);

        return v;
    }

    std::vector<gf::Vector2i> Board::filterMoveAuthorized_Check(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
        assert(coordCaseStart.y >= 0);
        assert(coordCaseStart.y < 8);
        assert(coordCaseStart.x >= 0);
        assert(coordCaseStart.x < 8);
        std::vector<gf::Vector2i> v;
        
        Piece piece = m_state[coordCaseStart.y * 8 + coordCaseStart.x].m_piece;

        for(const auto &coordCase : mAvailable) {

            std::size_t sizeBf = m_bin.size();
            movePieces(coordCaseStart, coordCase);
            std::size_t sizeAf = m_bin.size();

            if(!isInEchec(piece.getColor())) {
                v.push_back(coordCase);
            }

            deMovePieces(coordCaseStart, coordCase, sizeBf!=sizeAf);
        }

        //fonction tmp debug
        tmp(v);

        return v;
    }

    std::vector<gf::Vector2i> Board::addMoveBigSmallCastling(gf::Vector2i coordCaseStart, std::vector<gf::Vector2i> mAvailable) {
        assert(coordCaseStart.y >= 0);
        assert(coordCaseStart.y < 8);
        assert(coordCaseStart.x >= 0);
        assert(coordCaseStart.x < 8);
        
        std::vector<gf::Vector2i> v;
        
        Piece p = m_state[coordCaseStart.y * 8 + coordCaseStart.x].m_piece;
        
        if(p.getType() != ChessPiece::KING || coordCaseStart.x != 4) {
            return mAvailable;
        }
        
        Piece rookD = m_state[coordCaseStart.y * 8 + coordCaseStart.x+3].m_piece;
        Piece rookG = m_state[coordCaseStart.y * 8 + coordCaseStart.x-4].m_piece;
        
        if(p.m_isMoved) {
            return mAvailable;
        }

        // small castling
        if(p.getType() == ChessPiece::KING && rookD.getType() == ChessPiece::ROOK && rookD.getColor() == p.getColor()) {
            if(!rookD.m_isMoved) {
                
                bool piecesExist = false;

                for(int i = 1 ; i <= 2 ; i++) {
                    piecesExist |= (m_state[coordCaseStart.y * 8 + coordCaseStart.x+i].m_piece.getType() != ChessPiece::NONE);
                }
                
                if(!piecesExist) {
                    v.push_back(gf::Vector2i(coordCaseStart.x+1, coordCaseStart.y));
                    v.push_back(gf::Vector2i(coordCaseStart.x+2, coordCaseStart.y));
                    v.push_back(coordCaseStart);

                    bool inEchec = false;
                    for(const auto &coords : v) {
                        inEchec |= isInEchec(p.getColor(), coords);
                    }

                    if(!inEchec) {
                        mAvailable.push_back(gf::Vector2i(coordCaseStart.x+2, coordCaseStart.y));
                    }
                }
            }
        }

        v.clear();

        // big castling
        if(p.getType() == ChessPiece::KING && rookG.getType() == ChessPiece::ROOK && rookG.getColor() == p.getColor()) {
            if(!rookG.m_isMoved) {
            
                bool piecesExist = false;

                for(int i = 1 ; i <= 3 ; i++) {
                    piecesExist |= (m_state[coordCaseStart.y * 8 + coordCaseStart.x-i].m_piece.getType() != ChessPiece::NONE);
                }
                
                if(!piecesExist) {

                    v.push_back(gf::Vector2i(coordCaseStart.x-1, coordCaseStart.y));
                    v.push_back(gf::Vector2i(coordCaseStart.x-2, coordCaseStart.y));			
                    v.push_back(coordCaseStart);

                    bool inEchec = false;
                    for(auto coords = v.begin() ; coords != v.end() ; coords ++ ) {
                        inEchec |= isInEchec(p.getColor(), *coords);
                    }


                    if(!inEchec) {
                        gf::Log::debug("grand roque possible\n");
                        mAvailable.push_back(gf::Vector2i(coordCaseStart.x-2, coordCaseStart.y));
                    }
                }
            }
        }
        
        //fonction tmp debug
        tmp(mAvailable);
        
        return mAvailable;
    }

    void Board::movePieces(gf::Vector2i coord1, gf::Vector2i coord2) {
        assert(coord1.y >= 0);
        assert(coord1.y < 8);
        assert(coord1.x >= 0);
        assert(coord1.x < 8);
        assert(coord2.y >= 0);
        assert(coord2.y < 8);
        assert(coord2.x >= 0);
        assert(coord2.x < 8);
        assert(coord1 != coord2);
        
        Piece p1 = m_state[coord1.y * 8 + coord1.x].m_piece;
        Piece p2 = m_state[coord2.y * 8 + coord2.x].m_piece;

        //rock
        if(p1.getType() == ChessPiece::KING && abs(coord1.x - coord2.x) == 2) {
            
            std::swap(m_state[coord1.y * 8 + coord1.x].m_piece, m_state[coord2.y * 8 + coord2.x].m_piece);
            if(coord1.x - coord2.x < 0) {
                std::swap(m_state[coord1.y * 8 + 7].m_piece, m_state[coord2.y * 8 + coord2.x-1].m_piece);
            }else {
                std::swap(m_state[coord1.y * 8].m_piece, m_state[coord2.y * 8 + coord2.x+1].m_piece);
            }
            return;
        }

        // eat a piece
        if(p2.getType() != ChessPiece::NONE) {
            m_bin.push_back(p2);
            m_state[coord2.y * 8 + coord2.x].m_piece = Piece(ChessColor::NONE, ChessPiece::NONE);
        }
        
        std::swap(m_state[coord1.y * 8 + coord1.x].m_piece, m_state[coord2.y * 8 + coord2.x].m_piece);

        // prise en passant
        if(p1.getType() == ChessPiece::PAWN && p2.getType() == ChessPiece::NONE && coord1.x != coord2.x) {
            assert(m_state[coord1.y*8+coord2.x].m_piece.getType()==ChessPiece::PAWN);
            m_bin.push_back(m_state[coord1.y*8+coord2.x].m_piece);
            m_state[coord1.y*8+coord2.x].m_piece = Piece(ChessColor::NONE, ChessPiece::NONE);
            return;
        }
    }

    void Board::deMovePieces(gf::Vector2i coord1, gf::Vector2i coord2, bool inBin) {
        assert(coord1.y >= 0);
        assert(coord1.y < 8);
        assert(coord1.x >= 0);
        assert(coord1.x < 8);
        assert(coord2.y >= 0);
        assert(coord2.y < 8);
        assert(coord2.x >= 0);
        assert(coord2.x < 8);
        assert(coord1 != coord2);

        Piece p1 = m_state[coord1.y * 8 + coord1.x].m_piece;
        Piece p2 = m_state[coord2.y * 8 + coord2.x].m_piece;

        std::swap(m_state[coord1.y * 8 + coord1.x].m_piece, m_state[coord2.y * 8 + coord2.x].m_piece);
        
        if(p2.getType() == ChessPiece::PAWN  && p1.getType() == ChessPiece::NONE && inBin && coord1.x != coord2.x && m_coordTakePassing.y == coord1.y && m_coordTakePassing.x == coord2.x) {
            Piece pBin = m_bin.back();
            assert(pBin.getType() == ChessPiece::PAWN);
            m_state[coord1.y*8+coord2.x].m_piece = pBin;
            m_bin.pop_back();
        }else if(inBin) {
            Piece pBin = m_bin.back(); 
            m_state[coord2.y * 8 + coord2.x].m_piece = pBin;
            m_bin.pop_back();
        }
    }

    // coord case ou vont regarder lechec 
    // caseAvoid case qu'on vont veut pas regarder si cett cette case met echec 
    bool Board::isInEchec(ChessColor color, gf::Vector2i coord, gf::Vector2i caseAvoid) {
        if(coord == gf::Vector2i(-1)) { // case ou on cherche l'echec du roi de couleur color
            for(auto &caseP : m_state) {
                if(caseP.m_piece.getType() == ChessPiece::KING && caseP.m_piece.getColor() == color) {
                    coord = caseP.m_position;
                    break;
                }
            }
        }else {// on cherche une case dont les coordonees sont en parametre
            assert(coord.y >= 0);
            assert(coord.y < 8);
            assert(coord.x >= 0);
            assert(coord.x < 8);
        }

        for(auto & caseP : m_state) {
            if(caseP.m_piece.getType() != ChessPiece::NONE && caseP.m_piece.getColor() != color && caseP.m_position != caseAvoid) {
                
                std::vector<gf::Vector2i> casesAv = caseP.m_piece.getMoves(caseP.m_position);
                casesAv = filterMoveAuthorized_Tangled(caseP.m_position, casesAv);
                casesAv = filterMoveAuthorized_Pawn(caseP.m_position, casesAv);
                
                for(auto coordPass : casesAv) {
                    
                    if(coord == coordPass) {	
                         m_caseProvokeEchec = caseP.m_position;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void Board::tmp(std::vector<gf::Vector2i> a) {
        for(const auto &it : a) {
            assert(it.x >=0);
            assert(it.x<8);
            assert(it.y >=0);
            assert(it.y<8);
        }
    }

    std::string Board::getFen () {
        std::string fen = "";
        // 			Color |White|Black|Neutral|
        // K k Z: king 	|	Q q E: Queen	|	B b T: Bishop	|
        // N n Y: Knight|	R r U: Rook		|	P p J: Pawn		|
        // C c V: Camel |	I i X: Prince	|	D d A: Princess	|
        std::vector<std::string> white = { "K", "Q", "B", "N", "R", "P", "C", "I", "D" };
        std::vector<std::string> black = { "k", "q", "b", "n", "r", "p", "c", "i", "d" };
        std::vector<std::string> neutral = { "Z", "E", "T", "Y", "U", "J", "V", "X", "A" };

        int empty_case = 0;
        for (auto & c : m_state) {
            if (c.m_piece.getType() != ChessPiece::NONE) {
                if (empty_case > 0) {
                    fen += std::to_string(empty_case);
                }
            } else {
                empty_case++;
            }

            if (c.m_piece.getColor() == ChessColor::BLACK) {
                fen += black[(int)c.m_piece.getType()];
                empty_case = 0;
            } else if (c.m_piece.getColor() == ChessColor::WHITE) {
                fen += white[(int)c.m_piece.getType()];
                empty_case = 0;
            } else if (c.m_piece.getColor() == ChessColor::GREY) {
                fen += neutral[(int)c.m_piece.getType()];
                empty_case = 0;
            }

            if (c.m_position.x == 7) {
                if (empty_case > 0) {
                    fen += std::to_string(empty_case);
                }
                empty_case = 0;
                fen += "/";
            }
        }

        return fen;
    }

    ChessStatus Board::isGameOver (ChessColor col) {
        std::vector<gf::Vector2i> moves;
        bool has_move = false;
        for (auto & c : m_state) {
            if (c.m_piece.getType() != ChessPiece::NONE && c.m_piece.getColor() == col) {
                moves = c.m_piece.getMoves(c.m_position);
                moves = filterMoveAuthorized(c.m_position, moves); 
                if (!moves.empty()) {
                    has_move = true;
                    break;
                }
            }
        }

        if (!has_move) {
            if (isInEchec(col)) {
                return ChessStatus::WIN;
            }
            return ChessStatus::EQUALITY;
        }

        bool onlyKing = true;
        for (auto & c : m_state) {
            if (c.m_piece.getType() != ChessPiece::NONE && c.m_piece.getType() != ChessPiece::KING) {
                onlyKing = false;	
                break;
            }
        }

        if (onlyKing) {
            return ChessStatus::EQUALITY;
        }

        for (size_t i = 0; i < m_allPositions.size(); i++) {
            gf::Log::debug("-%s\n",m_allPositions[i].c_str());
            size_t count = 0;
            for (size_t j = i; j < m_allPositions.size(); j++) {
                
                if (m_allPositions[i] == m_allPositions[j] && i != j) {
                    gf::Log::debug("equalité entre le tour %li et le tour %li\n",i,j);
                    count++;
                }
            }
            if (count >= 3) {
                return ChessStatus::EQUALITY;
            }
        }
        return ChessStatus::ON_GOING;
    }

    void Board::promotionPiece(gf::Vector2i coord, ChessPiece p) {
        assert(coord.x >= 0);
        assert(coord.x < 8);
        assert(coord.y >= 0);
        assert(coord.y < 8);
        ChessColor c = m_state[coord.y * 8 + coord.x].m_piece.getColor();
        m_state[coord.y * 8 + coord.x].m_piece = Piece(c, p);
    }

    bool Board::hasPromotion() {
        for(auto &c : m_state) {
            Piece piece = c.m_piece;
            if(c.m_position.y == 0 && piece.getType()==ChessPiece::PAWN && piece.getColor()==ChessColor::WHITE) {
                return true;
            }
            if(c.m_position.y == 7 && piece.getType()==ChessPiece::PAWN && piece.getColor()==ChessColor::BLACK) {
                return true;
            }
        }

        return false;
    }

}