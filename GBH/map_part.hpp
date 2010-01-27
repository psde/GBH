#ifndef MAPPART_HPP
#define MAPPART_HPP

#include "include.hpp"

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 texcoord;
};

class MapPart
{
	private:
		boost::shared_ptr<VBO> vertices;
		boost::shared_ptr<VBO> texcoord;

	public:
		void pushVertex(Vertex &vert);
};

#endif