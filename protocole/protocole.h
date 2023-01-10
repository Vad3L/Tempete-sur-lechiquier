#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>
#include <gf/Vector.h>

#include "../model/chess.hpp"

using namespace gf::literals;


/* Types d'erreur */
typedef enum {
    NONE,     /* Validation de la requete */
    COUP_NO_VALIDE,      
    GAME_START,
    GAME_END
} CodeRep;

/* struct pos d'une pièce*/

using Pos = gf::Vector2i;

struct PartieRep {
    static constexpr gf::Id type = "PartieRep"_id;
    CodeRep err;                   /* Code de retour */
    ChessColor coulPion;
    ChessStatus status;
};

template<typename Archive>
Archive operator|(Archive& ar, PartieRep& data) {
    return ar | data.err | data.coulPion | data.status;
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
#endif // PROTOCOLE_H
