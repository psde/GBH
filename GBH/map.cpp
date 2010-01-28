#include "include.hpp"

Map::Map(const char *map, Style* style){
	this->style = style;

	Filereader* reader = new Filereader(map);

	this->numVertices = 0;

	Chunk *animation_data;
	Chunk *dmap_data;

	Chunk *chk;
	while((chk = reader->getNextChunk())){
		if(strncmp(chk->header->type,"ANIM",4) == 0) {
			animation_data = chk;
		}
		if(strncmp(chk->header->type,"DMAP",4) == 0) {
			dmap_data = chk;
		}
	}



	// Read animation data
	char* offset = animation_data->data;
	while(*reinterpret_cast<short*>(offset) < 992)
	{
		TileAnimation* anim = new TileAnimation;

		anim->base = *reinterpret_cast<short*>(offset);
		offset += sizeof(short);

		anim->frame_rate = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim->repeat = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim->anim_length = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim->unused = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim->tiles = reinterpret_cast<short*>(offset);
		offset += anim->anim_length * sizeof(short);

		animatedGeom[anim->base].curTile = 0;
		animatedGeom[anim->base].tick = 0;
		animatedGeom[anim->base].anim = anim;

		tileAnimations.push_back(anim);

		std::cout << anim->base << " " << (int)anim->frame_rate << " " << (int)anim->repeat << " " << (int)anim->anim_length << " " << anim->tiles[1] << std::endl;
	}

	// Read dmap data
	CompressedMap c_map;
	offset = dmap_data->data;
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

	for(int x=0;x<255;x++) for(int y=0;y<255;y++) 
	{
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
				
				if(animatedGeom.find(face.tile_number) != animatedGeom.end())
				{	
					int animationBase = face.tile_number;
					animatedGeom[animationBase].part.pushVertex(vertices[0]);
					animatedGeom[animationBase].part.pushVertex(vertices[1]);
					animatedGeom[animationBase].part.pushVertex(vertices[2]);
					animatedGeom[animationBase].part.pushVertex(vertices[3]);
				}
				else
				{
					geom[tex].pushVertex(vertices[0]);
					geom[tex].pushVertex(vertices[1]);
					geom[tex].pushVertex(vertices[2]);
					geom[tex].pushVertex(vertices[3]);
				}
			}
		}
	}
	delete reader;
}

Map::~Map()
{
}

void Map::draw()
{
	for(Part::iterator it = geom.begin(); it != geom.end(); it++)
	{
		int tex = (it->first > 1000 ? it->first - 1000 : it->first);
		glBindTexture(GL_TEXTURE_2D, this->style->getTexture(tex, (it->first > 1000)));
		it->second.draw();
	}

	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		//int tex = (it->first > 1000 ? it->first - 1000 : it->first);
		//it->second.anim.tiles[0]
		//std::cout << it->second.anim->tiles << std::endl;
		//TileAnimation anim = it->second.anim;
		//std::cout << anim.base << " " << (int)anim.frame_rate << " " << (int)anim.repeat << " " << (int)anim.anim_length << " " << anim.tiles[0] << std::endl;
		glBindTexture(GL_TEXTURE_2D, this->style->getTexture(it->first + it->second.curTile, false));
		it->second.part.draw();
	}
}

void Map::update()
{
	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		it->second.tick++;
		if(it->second.tick >= it->second.anim->frame_rate*10)
		{
			it->second.tick = 0;
			it->second.curTile = (++it->second.curTile)%2;
		}
	}
}

