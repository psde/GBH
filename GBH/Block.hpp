#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Gta2Formats.hpp"
#include "Style.hpp"
#include "VBO.hpp"
#include "main.hpp"

struct BlockFace{
	short tile_number;
	short lightning;
	short wall;
	short bullet_wall;
	short flat;
	short flip;
	short rotation_code;
};

struct Face
{
	short texture;
	VBO *vertices, *texcoord;
};

class Block {
	private:
		short groundType;
		short slopeType;

		static BlockFace *getBlockFace(int bitmap);
		BlockFace *blockFaces[5];
		BlockInfo block_info;

		Face *faces[5];
		
	public:
		Block(BlockInfo blck);
		~Block();

		bool isZero();

		void draw(int x, int y, int z, Style* style);
};

#endif
