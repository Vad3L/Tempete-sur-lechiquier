#ifndef PROTOCOL
#define PROTOCOL

#include <gf/Id.h>
#include <gf/SerializationOps.h>

using namespace gf::literals;

struct Paquet {
	static constexpr gf::Id type = "Paquet"_id;
	int choix;
};

template<typename Archive>
	Archive operator|(Archive& ar, Paquet& data) {
		return ar | data.choix;	
	}

#endif
