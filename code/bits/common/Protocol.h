#ifndef TSL_PROTOCOL_H
#define TSL_PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Chess.h"
#include "Card.h"

using namespace gf::literals;

namespace tsl {

    using Pos = gf::Vector2i;

    typedef enum {
        NONE, 
        COLOR,	
        COUP_NO_VALIDE,
        CARD_NO_VALIDE,
        GAME_START,
        GAME_END,
        NO_CARD,
        CARD
    } CodeRep;

    struct ColorDeck {
        static constexpr gf::Id type = "Part"_id;
        ChessColor colorPlayer;
        std::array<int, 5> hand;
    };

    
    template<typename Archive>
    Archive operator|(Archive& ar, ColorDeck& data) {
        return ar | data.colorPlayer | data.hand;
    }

    struct StartTurn {
        static constexpr gf::Id type = "StartTurn"_id;
        ChessColor colorPlayer;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, StartTurn& data) {
        return ar | data.colorPlayer;
    }

    struct StartGame {
        static constexpr gf::Id type = "StartGame"_id;
        CodeRep err;				   /* Code de retour */
    };

    template<typename Archive>
    Archive operator|(Archive& ar, StartGame& data) {
        return ar | data.err;
    }

    struct EndGame {
        static constexpr gf::Id type = "EndGame"_id;
        ChessStatus chessStatus;
        ChessColor chessColor;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, EndGame& data) {
        return ar | data.chessStatus | data.chessColor;
    }

    /***********************
    **********COUP**********
    ***********************/
    struct CoupRep {
        static constexpr gf::Id type = "CoupRep"_id;
        CodeRep err;				   /* Code de retour */
        Pos posStart;
        Pos posEnd;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, CoupRep& data) {
        return ar | data.err | data.posStart | data.posEnd;
    }

    /***********************
    **********PROMOTION**********
    ***********************/
    struct PromotionRep {
        static constexpr gf::Id type = "PromotionRep"_id;
        CodeRep err;				   /* Code de retour */
        Pos pos;
        ChessPiece choice;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PromotionRep& data) {
        return ar | data.err | data.pos | data.choice;
    }

    struct CardRep {
        static constexpr gf::Id type = "CardRep"_id;
        CodeRep err;
        size_t card; //num car dans mai
        int num; // num carte jouée
        std::vector<Pos> poses;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, CardRep& data) {
        return ar | data.err | data.card | data.poses | data.num;
    }

    template<typename Archive>
    Archive operator|(Archive& ar, Card& data) {
        return ar | data.m_num;
    }

    struct PartieRep {
        static constexpr gf::Id type = "PartieRep"_id;
        CodeRep err;				   /* Code de retour */
        ChessColor color;
        ChessStatus status;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, PartieRep& data) {
        return ar | data.err | data.color | data.status;
    }


    /***********************
    *****DEBUG PLATEAU******
    ***********************/
    struct Debug {
        static constexpr gf::Id type = "Debug"_id;
        std::string board;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, Debug& data) {
        return ar | data.board;
    }

    struct SendCard {
        static constexpr gf::Id type = "SendCard"_id;
        int num;
    };

    template<typename Archive>
    Archive operator|(Archive& ar, SendCard& data) {
        return ar | data.num;
    }

}

#endif // TSL_PROTOCOL_H