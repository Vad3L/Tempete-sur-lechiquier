#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <gf/Id.h>
#include <gf/SerializationOps.h>

using namespace gf::literals;

/* Identificateurs des requetes */
typedef enum { PARTIE, COUP } IdReq;


/* Types d'erreur */
typedef enum {
    ERR_OK,      /* Validation de la requete */
    ERR_PARTIE,  /* Erreur sur la demande de partie */
    ERR_COUP,    /* Erreur sur le coup joue */
    ERR_TYPE     /* Erreur sur le type de requete */
} TCodeRep;


struct TPartieReq {
    static constexpr gf::Id type = "TPartieReq"_id;
    IdReq idReq;               /* Identificateur de la requete */
    std::string nomJoueur;      /* Nom du joueur */
    //int  coulPion;             /* Couleur souhaitee du pion */
};

template<typename Archive>
Archive operator|(Archive& ar, TPartieReq& data) {
    return ar | data.idReq | data.nomJoueur; //| data.coulPion;
}

struct PartieRep {
    static constexpr gf::Id type = "PartieRep"_id;
    TCodeRep err;                   /* Code de retour */
    //std::string nomAdvers;         /* Nom du joueur adverse */
    //int  validCoulPion;       /* Validation de la couleur pour le pion */
    int coulPion;
};

template<typename Archive>
Archive operator|(Archive& ar, PartieRep& data) {
    return ar | data.err | data.coulPion;
}

/////////////////////

#endif //PROTOCOLE_H
