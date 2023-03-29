#include "AnnexFonctionServer.h"

#include <gf/Log.h>

namespace tsl {

    bool checkValidityCoup(Board& board, gf::Vector2i coordStart, gf::Vector2i coordEnd) {
        gf::Log::info("verifing coup\n");
        if (coordStart.x < 0 || coordStart.y < 0 || coordEnd.x > 7 || coordEnd.y > 7) {
            return false;
        }
        
        Piece pieceStart = board.m_state[coordStart.y * 8 + coordStart.x].m_piece;
        Piece p = board.m_state[coordEnd.y * 8 + coordEnd.x].m_piece;

        
        if(pieceStart.getColor() == ChessColor::NONE || pieceStart.getColor() != board.m_turnTo) {
            return false;
        }

        if(p.getType() == ChessPiece::KING) {
            return false;
        }
        
        std::vector<gf::Vector2i> moveAvailable = board.m_state[coordStart.y * 8 + coordStart.x].m_piece.getMoves(gf::Vector2i(coordStart.x, coordStart.y));
        moveAvailable = board.filterMoveAuthorized(gf::Vector2i(coordStart.x, coordStart.y), moveAvailable);
        
        auto it = std::find(moveAvailable.begin(), moveAvailable.end(), gf::Vector2i(coordEnd.x, coordEnd.y));
        return it != moveAvailable.end(); 
    }

    void checkCoupPacketValidity (Board& board, CoupRep& r) {
        assert(r.posStart.x >= 0);
        assert(r.posStart.x < 8);
        assert(r.posStart.y >= 0);
        assert(r.posStart.y < 8);

        assert(r.posEnd.x >= 0);
        assert(r.posEnd.x < 8);
        assert(r.posEnd.y >= 0);
        assert(r.posEnd.y < 8);

        bool coupValide = checkValidityCoup(board, r.posStart, r.posEnd);

        if (coupValide) {   
            r.err = CodeRep::NONE;
        } else {
            r.err = CodeRep::COUP_NO_VALIDE;
        }
    }

    void checkCardPacketValidity (Board& p, CardRep& r, std::array<Card, 5>& hand, Phase f) {
        gf::Log::debug("debut fonction card validity %i\n", r.err);
        if (r.err == CodeRep::NO_CARD) {
            return;
        }
        gf::Log::debug("j'ai recu une carte %i\n", r.err);

        assert(r.err != CodeRep::NO_CARD);
        assert(r.card <= 4);
        assert(r.err == CodeRep::CARD);

        r.num = hand[r.card].m_num;
        bool valide = true;
        if (!hand[r.card].m_isPlayable(p, f)) {
            gf::Log::debug("carte non jouable\n");
            valide = false;
        }
        
        if (valide) {
            r.err = CodeRep::NONE;
        } else {
            r.err = CodeRep::CARD_NO_VALIDE;
        } 
    }

    void checkPromotionValidity (Board& board, PromotionRep& r) {
        assert(r.pos.x >= 0);
        assert(r.pos.x < 8);
        assert(r.pos.y >= 0);
        assert(r.pos.y < 8);

        bool promoValide = false;
        Piece piece = board.m_state[r.pos.y * 8 + r.pos.x].m_piece;
        Case c = board.m_state[r.pos.y * 8 + r.pos.x];	

        std::cout << c.m_position.y << std::endl;
        std::cout << (piece.getColor() == ChessColor::WHITE ? "white" : "black") << std::endl;

        if (piece.getType() == ChessPiece::PAWN) {
            if (c.m_position.y == 0 && piece.getColor() == ChessColor::WHITE) {
                gf::Log::debug("Promotion Accepter du pion Blanc\n");
                promoValide = true;
            }

            if (c.m_position.y == 7 && piece.getColor() == ChessColor::BLACK) {
                gf::Log::debug("Promotion Accepter du pion noir\n");
                promoValide = true;
            }
        }

        if(!(r.choice == ChessPiece::QUEEN || r.choice == ChessPiece::ROOK || r.choice == ChessPiece::BISHOP || r.choice == ChessPiece::KNIGHT)) {
            gf::Log::error("Promotion invvalide : Type\n");
            promoValide = false;
        }

        if (promoValide) {
            r.err = CodeRep::NONE;
        } else {
            gf::Log::error("Tentative de promotion frauduleuse \n");
            r.err = CodeRep::COUP_NO_VALIDE;
        }

    }

    int receivingPacket (gf::TcpSocket& s, gf::Packet &p) {
        if (!s) {
            gf::Log::error("Client déconnecté\n");
            return -1;
        }

        if (gf::SocketStatus::Data != s.recvPacket(p)) {
            gf::Log::error("Lors de la réception du packet qui contient le coup du client\n");
            return -1;
        }

        return 0;
    }

    int sendingPacket (gf::TcpSocket& s, gf::Packet& p) {
        if (!s) {
            gf::Log::error("Client déconnecté");
            return -1;
        }

        if (gf::SocketStatus::Data != s.sendPacket(p)) {

            gf::Log::error("Lors de l'envoie du packet coupRep au client\n");
            return -1;
        }

        return 0;
    }

    int sendStartGame (gf::TcpSocket& a, gf::TcpSocket& b) {
        int ret = 0;
        gf::Packet packet;

        StartGame start;
        packet.is(start);
        
        if (gf::SocketStatus::Data != a.sendPacket(packet)) {
            gf::Log::error("Lors de l'envoie du packet de debut ou fin au client 1\n");
            ret = -1;
        }
        
        if (gf::SocketStatus::Data != b.sendPacket(packet)) {
            gf::Log::error("Lors de l'envoi du packet de debut fin au client 2\n");
            ret = -1;
        }

        return ret;
    }

    int sendEndGame (gf::TcpSocket& a, gf::TcpSocket& b, ChessStatus chessStatus, ChessColor chessColor) {
        int ret = 0;
        gf::Packet packet;

        EndGame end;
        end.chessStatus = chessStatus;
        end.chessColor = chessColor;
        packet.is(end);
        
        if (gf::SocketStatus::Data != a.sendPacket(packet)) {
            gf::Log::error("Lors de l'envoie du packet de debut ou fin au client 1\n");
            ret = -1;
        }
        
        if (gf::SocketStatus::Data != b.sendPacket(packet)) {
            gf::Log::error("Lors de l'envoi du packet de debut fin au client 2\n");
            ret = -1;
        }

        return ret;
    }

    int sendStartTurn (gf::TcpSocket& a, ChessColor m_turnTo){
        gf::Log::error("Envoir du debut du tour à %i\n", (int)m_turnTo);
        int ret = 0;
        gf::Packet packet;

        StartTurn start;
        start.colorPlayer = m_turnTo;
        packet.is(start);

        if (gf::SocketStatus::Data != a.sendPacket(packet)) {
            gf::Log::error("Lors de l'envoie du packet de debut de tour\n");
            ret = -1;
        }

        return ret;
    }

    int sendColorDeckCards(gf::TcpSocket& client, ChessColor c, std::array<Card, 5> hand) {
        gf::Packet pack;
            
        ColorDeck colorDeck;
        colorDeck.colorPlayer = c;
        std::array<int, 5> h;
        for(std::size_t i=0; i<5; i++) {
            h[i] = hand[i].m_num;
        }
        colorDeck.hand = h;
        pack.is(colorDeck);

        if (gf::SocketStatus::Data != client.sendPacket(pack)) {
            gf::Log::error("Lors de l'envoie du packet contenant la couleur au client\n");
            return -1;
        }

        return 0;
    }
    
    void sendCard (gf::TcpSocket& player, Card new_card) {
        gf::Packet pack;
        SendCard rep;
        rep.num = new_card.m_num;
        pack.is(rep);
	    sendingPacket(player, pack);
    }

    bool performCoup (Board& board, CoupRep& coup) {
        gf::Log::debug("------COUP VALIDE------\n");
        Piece p = board.m_state[coup.posStart.y * 8 + coup.posStart.x].m_piece;
        p.m_isMoved = true;
        board.movePieces(gf::Vector2i(coup.posStart.x, coup.posStart.y), gf::Vector2i(coup.posEnd.x, coup.posEnd.y));

        board.m_playerInEchec = board.isInEchec(!board.m_turnTo);
        
        board.prettyPrint();
        board.m_lastCoup.push_back(gf::Vector2i(coup.posStart.x,coup.posStart.y));
        board.m_lastCoup.push_back(gf::Vector2i(coup.posEnd.x,coup.posEnd.y));   
        gf::Log::debug("------COUP ------------------VALIDE------ %li\n", board.m_allPositions.size());
        board.m_allPositions.push_back(board.getFen());
        
        if (p.getType() == ChessPiece::PAWN && (coup.posEnd.y == 0 || coup.posEnd.y == 7)) {
            return true;
        }
        return false;
    }

    void performPromotion (Board& board, PromotionRep& promo) {
        gf::Log::debug("------PROMO VALIDE------\n");
        board.promotionPiece(gf::Vector2i(promo.pos.x, promo.pos.y), promo.choice);
        
        board.m_playerInEchec = board.isInEchec(!board.m_turnTo);
        
        gf::Log::debug("------PROMO -----------------------------------------VALIDE------%li\n", board.m_allPositions.size());
        board.m_allPositions.push_back(board.getFen());
        board.prettyPrint();
    }

    bool performCard (Board& board, CardRep& c, std::array<Card, 5>& hand, Deck& d) {
        gf::Log::debug("------CARD VALIDE------\n");
        bool works = hand[c.card].m_execute(board, c.poses);
        if (!works) {
            return false;
        }

        board.m_playerInEchec = board.isInEchec(!board.m_turnTo);

        board.m_allPositions.push_back(board.getFen());

        gf::Log::debug("La carte %s est joué \n", hand[c.card].m_name.c_str());
        board.prettyPrint();
        Card pick = d.getFirst();
        d.drop(hand[c.card]);
        hand[c.card] = pick;

        if (d.getNbCardInDeck() == 0) {
            d.emptyDiscard();
        }
        return true;
    }

    int performTurn (Deck& d, GamePhase& gp, Board& p, gf::TcpSocket& player, gf::TcpSocket& other, std::array<Card, 5>& hand, bool& promotion) {
        
        gf::Log::debug("début phase %i\n", (int)(gp.getCurrentPhase()));
        gf::Packet pack;
        if (receivingPacket(player, pack) == -1) {
            gf::Log::error("Erreur lors du reçu de la carte avant le coup\n");
            return -1;
        }

        if (gp.getCurrentPhase() == Phase::AVANT_COUP && pack.getType() == CoupRep::type) {
            gp.setCurrentPhase(Phase::COUP);
        }

        if (gp.getCurrentPhase() == Phase::AVANT_COUP) {
            gf::Log::debug("Phase avant coup\n");
            assert(pack.getType() == CardRep::type);
            CardRep card = pack.as<CardRep>();
            checkCardPacketValidity(p, card, hand, gp.getCurrentPhase());
            if (card.err == CodeRep::NONE) {
                gp.nextPhaseCard(hand[card.card]);
                bool worked = performCard(p, card, hand, d);
                if (!worked) {
                    gp.setCurrentPhase(Phase::AVANT_COUP);
                    return 2;
                } else {
                    sendCard(player, hand[card.card]);
                }
                if(p.hasPromotion()) {
                    gp.setCurrentPhase(Phase::COUP);
                    promotion = true;
                    gf::Log::error("\n\n promotion par carte (1)\n\n");
                }
            } else {
                return 2;
            }
            pack.is(card);
            if (sendingPacket(player, pack)) { return -1; }
            if (sendingPacket(other, pack)) { return -1; }
        } else if (gp.getCurrentPhase() == Phase::COUP) {
            if (promotion) {
                gf::Log::debug("Promotion\n");
                assert(pack.getType() == PromotionRep::type);
                PromotionRep promo = pack.as<PromotionRep>();
                checkPromotionValidity(p, promo);
                if (promo.err != CodeRep::NONE) {
                    return 2;
                }
                performPromotion(p, promo);
                if(gp.getNbCardPlay()>=gp.getLimNbCard()) {
                    gf::Log::error("\n\nfiiiiiiiiiiiiiiiiiiiiin 1 \n\n");
                    gp.setCurrentPhase(Phase::PAS_MON_TOUR);
                }else {
                    gp.setCurrentPhase(Phase::APRES_COUP);
                }
                promotion = false;
                pack.is(promo);
                if (sendingPacket(player, pack)) { return -1; }
                if (sendingPacket(other, pack)) { return -1; }
            } else {
                assert(pack.getType() == CoupRep::type);
                gf::Log::debug("Coup\n");
                CoupRep coup = pack.as<CoupRep>();
                checkCoupPacketValidity(p, coup);
                if (coup.err != CodeRep::NONE) {
                    return 2;
                }
                gf::Log::debug("Coup\n");
                promotion = performCoup(p, coup);
            
                if (!promotion) {
                    if(gp.getNbCardPlay()>=gp.getLimNbCard()) {
                        gp.setCurrentPhase(Phase::PAS_MON_TOUR);
                    }else {
                        gp.setCurrentPhase(Phase::APRES_COUP);
                    }
                }
                pack.is(coup);
                if (sendingPacket(player, pack)) { return -1; }
                if (sendingPacket(other, pack)) { return -1; }
            }
        } else if (gp.getCurrentPhase() == Phase::APRES_COUP) {
            assert(pack.getType() == CardRep::type);
            gf::Log::debug("Apres coup!\n");
            CardRep card = pack.as<CardRep>();
            checkCardPacketValidity(p, card, hand, gp.getCurrentPhase());
            if (card.err == CodeRep::NONE) {
                gp.nextPhaseCard(hand[card.card]);
                bool worked = performCard(p, card, hand, d);
                if (!worked) {
                    gp.setCurrentPhase(Phase::APRES_COUP);
                    return 2;
                } else {
                    sendCard(player, hand[card.card]);
                }
            } else if (card.err == CodeRep::NO_CARD) {
                gf::Log::error("\n\nfiiiiiiiiiiiiiiiiiiiiin no card\n\n");
                gp.setCurrentPhase(Phase::PAS_MON_TOUR);
            } 

            pack.is(card);
            if(card.err != CodeRep::NO_CARD) {
                if (sendingPacket(player, pack)) { return -1; }
                if (sendingPacket(other, pack)) { return -1; }
            }
            
            if(p.hasPromotion()) {
                gp.setCurrentPhase(Phase::COUP);
                promotion = true;
                gf::Log::error("\n\n promotion par carte (2)\n\n");
            }
        }

        if (gp.getCurrentPhase() == Phase::PAS_MON_TOUR) {
            gf::Log::debug("tour finito\n");
            return 0;
        }
        
        return 2;
    }
}