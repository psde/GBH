#include "include.hpp"

Map::Map(const char *map, Style* style){
	this->style = style;

	Filereader* reader = new Filereader(map);

	Chunk* chk;
	while((chk = reader->getNextChunk())){
		if(strncmp(chk->header->type,"DMAP",4) == 0) {
			CompressedMap c_map;
			char* offset = chk->data;
			c_map.base = reinterpret_cast<int*>(offset);
			offset += 256 * 256 * sizeof(int);

			c_map.column_words = *reinterpret_cast<int*>(offset);
			offset += sizeof(int);

			c_map.columns = reinterpret_cast<int*>(offset);
			offset += c_map.column_words * sizeof(int);

			c_map.num_blocks = *reinterpret_cast<int*>(offset);
			offset += sizeof(int);

			c_map.blocks = reinterpret_cast<BlockInfo*>(offset);
			offset += c_map.num_blocks * sizeof(BlockInfo);

			Block* block_zero = new Block(c_map.blocks[0]);

			for(int x=0;x<255;x++) for(int y=0;y<255;y++) for(int z=0;z<7;z++){
				this->citySphere[x][y][z] = block_zero;
			}

			for(int x=0;x<255;x++) for(int y=0;y<255;y++) {
				int base = c_map.base[y*256+x];

				ColInfo* column = reinterpret_cast<ColInfo*>(c_map.columns + base);

				int i = 0;

				for(i=0;i<(column->height-column->offset);i++){
					this->citySphere[x][y][i+column->offset] = new Block(c_map.blocks[column->blockd[i]]);
				}
			}
		}
		free(chk);
	}
	delete reader;
}

Map::~Map()
{
	//delete[] this->citySphere;
    for(int z=0;z<7;z++){
		for(int x=0;x<255;x++){
			for(int y=0;y<255;y++){
				if(!this->citySphere[x][y][z]->isZero()){
					delete this->citySphere[x][y][z];
				}
			}
		}
    }
}

void Map::draw()
{
	int _drawrange = 100;
	int _start = 0;
	for(int z=0;z<7;z++){
		for(int x=_start;x<_drawrange+_start;x++){
			for(int y=_start;y<_drawrange+_start;y++){
				if(!this->citySphere[x][y][z]->isZero()){
					this->citySphere[x][y][z]->draw(x, y, z, this->style);
				}
			}
		}
	}
}

void Map::update()
{
	
}

