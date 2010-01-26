#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Gta2Formats.hpp"
#include "Style.hpp"
#include "main.hpp"

struct block_face{
	short tile_number;
	short lightning;
	short wall;
	short bullet_wall;
	short flat;
	short flip;
	short rotation_code;
};

class Block {
private:
	short ground_type;
	short slope_type;

	static block_face* getBlockFace(int bitmap);
	block_face *faces[5];
	BlockInfo block_info;
	
public:
	Block(BlockInfo blck);
	~Block();

	bool isZero();

	void draw(int x, int y, int z, Style* style);
};

#endif
