#ifndef MAP_HPP
#define MAP_HPP

#include "Block.hpp"
#include "main.hpp"

class Map
{
private:
	//vector<VBO*> vbos;
	//BlockInfo city_sphere[256][256][8];
	Block* city_sphere[256][256][8];
	Style* style;

public:
	Map(const char * map, Style*);
	~Map();
	//bool loadMap(const char * map, Style*);

	void draw();
	void update();
};

#endif