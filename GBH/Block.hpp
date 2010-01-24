#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "BlockReader.hpp"
#include "Gta2Formats.hpp"
#include "Style.hpp"
#include "main.hpp"

struct _block_face{
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

	static BlockReader* block_reader;

	//VBO* vbos[5];
	int numVBOs;

	short ground_type;
	short slope_type;

	bool VBOsBuild;

	_block_face *faces[5];
public:
	BlockInfo block_info;
	Block(BlockInfo blck);
	~Block();

	bool isZero();

	void buildVBOs(int, int, int, Style*);
	void draw(int x, int y, int z, Style* style);

	//int getNumVBOs();
	//VBO *getVBO(int);
	//VBO_Vert* getVertices(int);
};

#endif