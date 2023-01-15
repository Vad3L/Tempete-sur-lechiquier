#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>
#include <gf/Vector.h>

#include "../model/Chess.hpp"
#include "../model/Card.hpp"

using namespace gf::literals;

typedef enum {
    NONE, 
    COLOR,    
    COUP_NO_VALIDE,      
    GAME_START,
    GAME_END
} CodeRep;

/* struct pos d'une pièce*/

using Pos = gf::Vector2i;

struct PartieRep {
    static constexpr gf::Id type = "PartieRep"_id;
    CodeRep err;                   /* Code de retour */
    ChessColor colorPion;
    ChessStatus status;
};

template<typename Archive>
Archive operator|(Archive& ar, PartieRep& data) {
    return ar | data.err | data.colorPion | data.status;
}

/***********************
**********COUP**********
***********************/
struct CoupRep {
    static constexpr gf::Id type = "CoupRep"_id;
    CodeRep err;                   /* Code de retour */
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
    CodeRep err;                   /* Code de retour */
    Pos pos;
    ChessPiece choice;
};

template<typename Archive>
Archive operator|(Archive& ar, PromotionRep& data) {
    return ar | data.err | data.pos | data.choice;
}


/***********************
**********DISTRIB CARD**********
***********************/
struct DistribRep {
    static constexpr gf::Id type = "DistribRep"_id;
    CodeRep err;                   /* Code de retour */
    std::vector<Card> hand;
};

template<typename Archive>
Archive operator|(Archive& ar, DistribRep& data) {
    return ar | data.err | data.hand;
}

template<typename Archive>
Archive operator|(Archive& ar, Card& data) {
    return ar | data.m_num;
}
#endif // PROTOCOLE_H
