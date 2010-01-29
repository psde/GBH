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
	char* starting = animation_data->data;
	char* offset = animation_data->data;
	while((int)offset - (int)starting < animation_data->header->size )
	{
		/*std::cout << (int)offset << " " << (int)starting << " " << (int)offset - (int)starting << " " << animation_data->header->size << std::endl;
		system("pause");*/
		TileAnimation anim;

		anim.base = *reinterpret_cast<short*>(offset);
		offset += sizeof(short);

		anim.frame_rate = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim.repeat = *reinterpret_cast<unsigned char*>(offset);
		offset += sizeof(unsigned char);

		anim.anim_length = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		//anim.unused = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim.tiles = reinterpret_cast<short*>(offset);
		offset += anim.anim_length * sizeof(short);

		std::vector<int> animationTiles;

		for(int i=0; i<anim.anim_length;i++)
		{
			animationTiles.push_back(anim.tiles[i]);
		}

		animatedGeom[anim.base].curTile = 0;
		animatedGeom[anim.base].tick = 0;
		animatedGeom[anim.base].anim = anim;
		animatedGeom[anim.base].animationTiles = animationTiles;

		animatedGeom[anim.base+1000].curTile = 0;
		animatedGeom[anim.base+1000].tick = 0;
		animatedGeom[anim.base+1000].anim = anim;
		animatedGeom[anim.base+1000].animationTiles = animationTiles;

		tileAnimations.push_back(anim);

		std::cout << anim.base << " " << (int)anim.frame_rate << " " << (int)anim.repeat << " " << (int)anim.anim_length << " " << anim.tiles[1] << " " << animationTiles.size() << std::endl;
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
			this->addBlock(c_map.blocks[column->blockd[i]], Vector3(x, -y, i+column->offset));
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
		int tex = (it->first > 1000 ? it->first - 1000 : it->first);

		tex = it->second.animationTiles[it->second.curTile];

		glBindTexture(GL_TEXTURE_2D, this->style->getTexture(it->second.animationTiles[it->second.curTile], (it->first > 1000)));
		it->second.part.draw();
	}
}

void Map::update()
{
	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		it->second.tick++;
		if(it->second.tick >= it->second.anim.frame_rate*2)
		{
			it->second.tick = 0;
			it->second.curTile++;
			if(it->second.curTile >= it->second.anim.anim_length)
			{
				it->second.curTile = 0;
			}
		}
	}
}

void Map::addBlock(BlockInfo &block, Vector3 &offset)
{
	BlockFace faces[5];
	faces[0] = Block::getBlockFace(block.top);
	faces[1] = Block::getBlockFace(block.right);
	faces[2] = Block::getBlockFace(block.bottom);
	faces[3] = Block::getBlockFace(block.left);
	faces[4] = Block::getBlockFace(block.lid);

	Quad<Vector3> lid;
	lid.tl = Vector3(offset.x + 0.0f, offset.y + 0.0f, offset.z + 1.0f);
	lid.tr = Vector3(offset.x + 1.0f, offset.y + 0.0f, offset.z + 1.0f);
	lid.bl = Vector3(offset.x + 1.0f, offset.y + 1.0f, offset.z + 1.0f);
	lid.br = Vector3(offset.x + 0.0f, offset.y + 1.0f, offset.z + 1.0f);

	Quad<Vector3> top;
	top.tl = Vector3(offset.x + 0.0f, offset.y + 1.0f, offset.z + 0.0f);
	top.tr = Vector3(offset.x + 1.0f, offset.y + 1.0f, offset.z + 0.0f);
	top.bl = lid.bl;
	top.br = lid.br;

	Quad<Vector3> bottom;
	bottom.tl = Vector3(offset.x + 0.0f, offset.y + 0.0f, offset.z + 0.0f);
	bottom.tr = Vector3(offset.x + 1.0f, offset.y + 0.0f, offset.z + 0.0f);
	bottom.bl = lid.tr;
	bottom.br = lid.tl;

	Quad<Vector3> right;
	right.tl = Vector3(offset.x + 1.0f, offset.y + 1.0f, offset.z + 0.0f);
	right.tr = Vector3(offset.x + 1.0f, offset.y + 0.0f, offset.z + 0.0f);
	right.bl = lid.tr;
	right.br = lid.bl;

	Quad<Vector3> left;
	left.tl = Vector3(offset.x + 0.0f, offset.y + 0.0f, offset.z + 0.0f);
	left.tr = Vector3(offset.x + 0.0f, offset.y + 1.0f, offset.z + 0.0f);
	left.bl = lid.br;
	left.br = lid.tl;


	if(faces[2].flat && !faces[0].flat)
	{
		top = bottom;
		faces[0].flat = true;
		faces[2].flip = faces[0].flip;
	}

	if(faces[0].flat && !faces[2].flat)
	{
		bottom = top;
		faces[2].flat = true;
		faces[0].flip = faces[2].flip;
	}

	if(faces[1].flat && !faces[3].flat)
	{
		left = right;
		faces[3].flat = true;
		faces[1].flip = faces[3].flip;
	}

	if(faces[3].flat && !faces[1].flat)
	{
		right = left;
		faces[1].flat = true;
		faces[3].flip = faces[1].flip;
	}

	this->addFace(faces[0], top);
	this->addFace(faces[1], right);
	this->addFace(faces[2], bottom);
	this->addFace(faces[3], left);
	this->addFace(faces[4], lid);
}

void Map::addFace(BlockFace &face, Quad<Vector3> &quad)
{
	if(face.tile_number <= 0) return;

	std::vector<Vertex> vertices(4);
	vertices[0].coord = quad.tl;
	vertices[1].coord = quad.tr;
	vertices[2].coord = quad.bl;
	vertices[3].coord = quad.br;

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
		int texture = face.tile_number + (face.flat ? 1000 : 0);
		animatedGeom[texture].part.pushVertex(vertices[0]);
		animatedGeom[texture].part.pushVertex(vertices[1]);
		animatedGeom[texture].part.pushVertex(vertices[2]);
		animatedGeom[texture].part.pushVertex(vertices[3]);
	}
	else
	{
		int texture = face.tile_number + (face.flat ? 1000 : 0);
		geom[texture].pushVertex(vertices[0]);
		geom[texture].pushVertex(vertices[1]);
		geom[texture].pushVertex(vertices[2]);
		geom[texture].pushVertex(vertices[3]);
	}
}


/*Matrix4x4 mat(Vector2(0.01, 0.01), Vector2(0.99, 0.01), Vector2(0.99, 0.99), Vector2(0.01, 0.99));

if(face.flip == 0)
	mat = Matrix4x4::scale(Vector3(-1, 1, 1)) * mat;	

mat = Matrix4x4::rotationMatrixZ(Math::degreesToRadians(180 - (face.rotation_code * 90))) * mat;

vertices[0].texcoord = Vector2(mat.f[0][0], mat.f[0][1]);
vertices[1].texcoord = Vector2(mat.f[1][0], mat.f[1][1]);
vertices[2].texcoord = Vector2(mat.f[2][0], mat.f[2][1]);
vertices[3].texcoord = Vector2(mat.f[3][0], mat.f[3][1]);*/