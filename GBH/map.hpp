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

struct AnimationMapPart
{
	MapPart part;
	TileAnimation *anim;
	int curTile;
	int tick;
};

class Map
{
	// Texture, MapPart
	typedef std::map<int, MapPart> Part;

	// Basetile of Animation, AnimationMapPart
	typedef std::map<int, AnimationMapPart> AnimatedPart;

	private:
		Block* citySphere[256][256][8];
		Style* style;

		Part geom;
		AnimatedPart animatedGeom;

		std::vector<TileAnimation*> tileAnimations; //meh?
	public:
		Map(const char * map, Style*);
		~Map();

		void draw();
		void update();

		long numVertices;
};

#endif