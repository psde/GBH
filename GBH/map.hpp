#ifndef MAP_HPP
#define MAP_HPP

#include "include.hpp"

template<class T> 
	struct Quad {
		T tl, tr, bl, br;
	};

struct Texture
{
	Texture(int t, bool f)
		: tex(t), flat(f)
	{ }

	int tex;
	bool flat;
};

class Map
{
	typedef std::map<int, MapPart> Part;

	private:
		Block* citySphere[256][256][8];
		Style* style;

		Part geom;

	public:
		Map(const char * map, Style*);
		~Map();

		void draw();
		void update();
};

#endif