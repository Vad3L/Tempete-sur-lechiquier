#ifndef TSL_PROTOCOL_H
#define TSL_PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>

#include "Chess.h"
#include "Card.h"

using namespace gf::literals;

namespace tsl {

    /*
    * server -> client
    */

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
    };

    template<typename Archive>
    Archive operator|(Archive& ar, StartGame& data) {
        return ar;
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

    /*
    * client -> server
    */



}

#endif // TSL_PROTOCOL_H