#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>

using namespace gf::literals;


/* Types d'erreur */
typedef enum {
    OK,      /* Validation de la requete */
} TCodeRep;


struct PartieRep {
    static constexpr gf::Id type = "PartieRep"_id;
    TCodeRep err;                   /* Code de retour */
    int coulPion;
};

template<typename Archive>
Archive operator|(Archive& ar, PartieRep& data) {
    return ar | data.err | data.coulPion;
}

#endif // PROTOCOLE_H
