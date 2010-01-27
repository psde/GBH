#ifndef MAP_HPP
#define MAP_HPP

#include "include.hpp"

class Map
{
private:
	Block* citySphere[256][256][8];
	Style* style;

public:
	Map(const char * map, Style*);
	~Map();

	void draw();
	void update();
};

#endif