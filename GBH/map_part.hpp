#ifndef MAPPART_HPP
#define MAPPART_HPP

#include "include.hpp"

struct Vertex {
	Vector3 coord;
	Vector2 texcoord;
};

class MapPart
{
	private:
		int lastModified, lastUpdate;

		std::vector<Vector3> coord;
		std::vector<Vector2> texcoord;

		boost::shared_ptr<VBO> coordVbo;
		boost::shared_ptr<VBO> texcoordVbo;

	public:
		MapPart();

		void pushVertex(Vertex &vert);
		void draw();

		long getSize();
};

#endif