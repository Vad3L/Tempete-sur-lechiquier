#ifndef CPIECE_H
#define CPIECE_H

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/Vector.h>

class CPiece {
	private: 
		gf::Vector2f m_position;
	public:
		CPiece(const gf::Vector2f& mPosition);
		void render(gf::RenderTarget& target);
};

#endif //CPIECE_H