#include "serverTools.hpp"

std::vector<PartieRep> buildRepPartie(TPartieReq pr1, TPartieReq pr2) {

    std::vector<PartieRep> partieRep;

    PartieRep rep1;
    rep1.err = ERR_OK;
    rep1.coulPion = 1;

    partieRep.push_back(rep1);

    PartieRep rep2;
    rep2.err = ERR_OK;
    rep2.coulPion = -1;

    partieRep.push_back(rep2);

    return partieRep;
}
