#ifndef BLOCKREADER_HPP
#define BLOCKREADER_HPP

#include "main.hpp"

struct BlockStructure{
	//VBO_Vert *VBO_Vertices[5];
	//VBO_TexCoord *VBO_Tex[5];
	int size[5];
	bool hasStuff[5];
};

class BlockReader {
	private:
		BlockStructure *block_structure[64];

	public:
		BlockReader();

		void readBlocks();

		//VBO_Vert *getVertices(int block_number, int vert_number, int x_offset, int y_offset, int z_offset);
		//VBO_TexCoord *getTexCoord(int block_number, int vert_number);
		int getCount(int block_number, int vert_number);

		bool hasLeft();
		bool hasRight();
		bool hasTop();
		bool hasBottom();
		bool hasLid();

};

#endif