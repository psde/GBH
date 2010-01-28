#include "include.hpp"

Map::Map(const char *map, Style* style){
	this->style = style;

	Filereader* reader = new Filereader(map);

	this->numVertices = 0;

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

			for(int x=0;x<255;x++) for(int y=0;y<255;y++) {
				int base = c_map.base[y*256+x];

				ColInfo* column = reinterpret_cast<ColInfo*>(c_map.columns + base);

				int i = 0;

				for(i=0;i<(column->height-column->offset);i++){
					BlockFace face = Block::getBlockFace(c_map.blocks[column->blockd[i]].lid);
					int tex = face.tile_number + (face.flat ? 1000 : 0);

					if(face.tile_number > 0) {
						std::vector<Vertex> vertices(4);
						int z = i+column->offset;
						vertices[0].coord = Vector3(x + 0.0f, -y + 0.0f, z + 0.0f);
						vertices[1].coord = Vector3(x + 1.0f, -y + 0.0f, z + 0.0f);
						vertices[2].coord = Vector3(x + 1.0f, -y + 1.0f, z + 0.0f);
						vertices[3].coord = Vector3(x + 0.0f, -y + 1.0f, z + 0.0f);

						// We need our texture set to GL_WRAP_AROUND for this to work
						// Obviously, pixels from the other side are leaking around when filtering
						// So this offset tries to eleminate that - maybe fix the matrix to avoid this
						// (Still happens on the mipmapped, distance tile textures)
						vertices[0].texcoord = Vector2(0.01, 0.01);
						vertices[1].texcoord = Vector2(0.99, 0.01);
						vertices[2].texcoord = Vector2(0.99, 0.99);
						vertices[3].texcoord = Vector2(0.01, 0.99);

						// THIS is weird.
						if(face.flip == 0)
							for(std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++)
								it->texcoord = Matrix4x4::scale(Vector3(-1, 1, 1)) * it->texcoord;

						for(std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); it++)
						{							
							it->texcoord = Matrix4x4::rotationMatrixZ(Math::degreesToRadians(180 - (face.rotation_code * 90))) * it->texcoord;
						}

						geom[tex].pushVertex(vertices[0]);
						geom[tex].pushVertex(vertices[1]);
						geom[tex].pushVertex(vertices[2]);
						geom[tex].pushVertex(vertices[3]);
					}
				}
			}
		}
		free(chk);
	}
	delete reader;
}

Map::~Map()
{
}

void Map::draw()
{
	this->numVertices = 0;
	for(Part::iterator it = geom.begin(); it != geom.end(); it++)
	{
		int tex = (it->first > 1000 ? it->first - 1000 : it->first);
		glBindTexture(GL_TEXTURE_2D, this->style->getTexture(tex, (it->first > 1000)));
		it->second.draw();
		this->numVertices += 4 * it->second.getSize();
	}
}

void Map::update()
{
	
}

