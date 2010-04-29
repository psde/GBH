#include "include.hpp"

// ============== Just for testing:
GLuint fboId;
GLuint depthTextureId;
int shadowMapWidth = 1024*4;
int shadowMapHeight = 1024*4;
// ==============


Map::Map(const char *map, Style* style){
	this->style = style;

	Filereader* reader = new Filereader(map);

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
		TileAnimation anim;

		anim.base = *reinterpret_cast<short*>(offset);
		offset += sizeof(short);

		anim.frame_rate = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		anim.repeat = *reinterpret_cast<unsigned char*>(offset);
		offset += sizeof(unsigned char);

		anim.anim_length = *reinterpret_cast<char*>(offset);
		offset += sizeof(char);

		// unused char in file structure
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

	GLenum FBOstatus;
	
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); 
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
	
	FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FBO error: " << FBOstatus << std::endl;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

Map::~Map()
{
}

void Map::drawGeometry()
{
	glActiveTexture(GL_TEXTURE0);
	for(Part::iterator it = geom.begin(); it != geom.end(); it++)
	{
		glBindTexture(GL_TEXTURE_2D, this->style->getTexture((it->first > 1000 ? it->first - 1000 : it->first), (it->first > 1000)));
		it->second.draw();
	}

	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		glBindTexture(GL_TEXTURE_2D, this->style->getTexture(it->second.animationTiles[it->second.curTile], (it->first > 1000)));
		it->second.part.draw();
	}
}

void Map::draw(Gosu::Graphics &graphics)
{


	//this->drawGeometry();


	//return;
	Gosu::PostProcessing local(graphics);
	static Gosu::ShaderProgram shadowMapProgram = local.compile(L"data/shader/shadowMap.frag");
	static Gosu::ShaderProgram shadowProgram = local.compile(L"data/shader/shadow.frag", L"data/shader/shadow.vert");

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	// render from the light pov to the shadow fbo
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 

	glViewport(0, 0, shadowMapWidth, shadowMapHeight);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, shadowMapWidth/shadowMapHeight, 200.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt(0, 0, 250, 128, -128, 0, 0, 0, 1);

	glUseProgram(shadowMapProgram.program);

	glUniform1i(glGetUniformLocation(shadowMapProgram.program, "colorTexture"), 0); 

	// Save matrix to texture
	float modelView[16];
	float projection[16];


	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	
	glMatrixMode(GL_TEXTURE);
	glActiveTexture(GL_TEXTURE7);
	
	glLoadIdentity();	

	//gluLookAt(0, 0, 250, 128, -128, 0, 0, 0, 1);

	glMultMatrixf(projection);
	glMultMatrixf(modelView);
	
	glMatrixMode(GL_MODELVIEW);

    this->drawGeometry();
	
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//gluPerspective(45.0f, (Gosu::realWidth(graphics)/Gosu::realHeight(graphics)), 200.0f, 1000.0f);

	// render normal camera view
	glViewport(0, 0, Gosu::realWidth(graphics), Gosu::realHeight(graphics));
	
	glUseProgram(shadowProgram.program);

	glUniformMatrix4fv(glGetUniformLocation(shadowProgram.program, "test"), 16, false, projection); 

	glUniform1i(glGetUniformLocation(shadowProgram.program, "colorTexture"), 0); 

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glUniform1i(glGetUniformLocation(shadowProgram.program, "shadowMap"), 7); 

	//setTextureMatrix();
    this->drawGeometry();

	glUseProgram(0);


	// Debug code to visualise depth buffer onscreen
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1280/2,1280/2,-1024/2,1024/2,1,20);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor4f(1,1,1,1);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	glTranslated(0,0,-1);
	
	glBegin(GL_QUADS);
		glTexCoord2d(0,0);
		glVertex3f(0,0,0);

		glTexCoord2d(1,0);
		glVertex3f(1280/2,0,0);

		glTexCoord2d(1,1);
		glVertex3f(1280/2,1024/2,0);

		glTexCoord2d(0,1);
		glVertex3f(0,1024/2,0);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();



}

void Map::update()
{
	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		it->second.tick++;
		if(it->second.tick >= it->second.anim.frame_rate*2)
		{
			it->second.tick = 0;
			it->second.curTile = (++it->second.curTile)%it->second.anim.anim_length;
		}
	}
}

long Map::getVerticeSize()
{
	long size=0;
	for(Part::iterator it = geom.begin(); it != geom.end(); it++)
	{
		size += it->second.getVerticeSize();
	}

	for(AnimatedPart::iterator it = animatedGeom.begin(); it != animatedGeom.end(); it++)
	{
		size += it->second.part.getVerticeSize();
	}
	return size;
};

Quad<Vector3> Map::buildSlopeLid(int slope, int steps)
{
	float height = 1.0 / steps;
	int level = slope % steps;
	float low = 1 + height * level - height * steps;

	Quad<Vector3> lid;
	lid.tl = Vector3(0, 0, low);
	lid.tr = Vector3(1, 0, low);
	lid.bl = Vector3(1, 1, low);
	lid.br = Vector3(0, 1, low);

	switch(slope / steps)
	{
		case 0: // up
			lid.bl += Vector3(0, 0, height);
			lid.br += Vector3(0, 0, height);
			break;
		case 1: // down
			lid.tl += Vector3(0, 0, height);
			lid.tr += Vector3(0, 0, height);
			break;
		case 2: // right
			lid.tl += Vector3(0, 0, height);
			lid.br += Vector3(0, 0, height);
			break;
		case 3: // left
			lid.tr += Vector3(0, 0, height);
			lid.bl += Vector3(0, 0, height);
			break;
	}

	return lid;
}

void Map::addBlock(BlockInfo &block, Vector3 &offset)
{
	int slope = (block.slope_type & 252) >> 2;

	BlockFace faces[5];
	faces[0] = Block::getBlockFace(block.top);
	faces[1] = Block::getBlockFace(block.right);
	faces[2] = Block::getBlockFace(block.bottom);
	faces[3] = Block::getBlockFace(block.left);
	faces[4] = Block::getBlockFace(block.lid);

	Vector2 low(0.0f, 0.0f);
	Vector2 high(1.0f, 1.0f);

	if(slope == 61 )
	{
		low = Vector2(0.35f, 0.35f);
		high = Vector2(0.65f, 0.65f);
	}

	if(slope == 53) // partial left
		high.x = 0.3f;

	if(slope == 54) // partial right
		low.x = 0.7f;

	if(slope == 55) // partial top
		low.y = 0.7f;

	if(slope == 56) // partial bottom
		high.y = 0.3f;

	if(slope == 57) // partial top left
	{
		high.x = 0.3f;
		low.y = 0.7f;
	}

	if(slope == 58) // partial top right
	{
		low.x = 0.7f;
		low.y = 0.7f;
	}

	if(slope == 59) // partial bottom right
	{
		high.y = 0.3f;
		low.x = 0.7f;
	}
	if(slope == 60) // partial bottom left
	{
		high.y = 0.3f;
		high.x = 0.3f;
	}

	Quad<Vector3> lid;
	lid.tl = Vector3(low.x,  low.y, 1);
	lid.tr = Vector3(high.x, low.y, 1);
	lid.bl = Vector3(high.x, high.y, 1); // what the hell, this is bottom right?!
	lid.br = Vector3(low.x,  high.y, 1);

	if(slope >= 1 && slope <= 8)
		lid = this->buildSlopeLid(slope - 1, 2);
	if(slope >= 9 && slope <= 40)
		lid = this->buildSlopeLid(slope - 9, 8);
	if(slope >= 41 && slope <= 44)
		lid = this->buildSlopeLid(slope - 41, 1);

	if(slope == 52) 
	{
		lid.tl = Vector3(0, 1, 1);
		lid.tr = Vector3(0, 1, 1);
		lid.bl = Vector3(0, 1, 1); 
		lid.br = Vector3(0, 1, 1);
	}

	Quad<Vector3> top;
	top.tl = Vector3(high.x, high.y, 0.0f);
	top.tr = Vector3(low.x, high.y, 0.0f);
	top.bl = lid.br;	
	top.br = lid.bl;

	Quad<Vector3> bottom;
	bottom.tl = Vector3(low.x, low.y, 0.0f);
	bottom.tr = Vector3(high.x, low.y, 0.0f);
	bottom.bl = lid.tr;
	bottom.br = lid.tl;

	Quad<Vector3> right;
	right.tl = Vector3(high.x, low.y, 0.0f);
	right.tr = Vector3(high.x, high.y, 0.0f);
	right.bl = lid.bl;
	right.br = lid.tr;

	Quad<Vector3> left;
	left.tl = Vector3(low.x, high.y, 0.0f);
	left.tr = Vector3(low.x, low.y, 0.0f);
	left.bl = lid.tl;
	left.br = lid.br;

	if(slope == 52)
	{
		right.tl = Vector3(0, low.y, 0.0f);
	}

	// flat-fliping seems correct now
	if(faces[2].flat && !faces[0].flat && faces[0].tile_number != 0)
	{
		top = bottom;
		faces[2].tile_number = 0;
		faces[0].flat = true;
		faces[0].flip = !faces[0].flip;
	}

	if(faces[0].flat && !faces[2].flat && faces[2].tile_number != 0)
	{
		bottom = top;
		faces[0].tile_number = 0;
		faces[2].flat = true;
		faces[2].flip = !faces[2].flip;
	}

	if(faces[1].flat && !faces[3].flat && faces[3].tile_number != 0)
	{
		left = right;
		faces[1].tile_number = 0;
		faces[3].flat = true;
		faces[3].flip = !faces[3].flip;
	}

	if(faces[3].flat && !faces[1].flat && faces[1].tile_number != 0)
	{
		right = left;
		faces[3].tile_number = 0;
		faces[1].flat = true;
		faces[1].flip = !faces[1].flip;
	}

	this->addFace(faces[0], top, offset, low, high);
	this->addFace(faces[1], right, offset, low, high);
	this->addFace(faces[2], bottom, offset, low, high);
	this->addFace(faces[3], left, offset, low, high);
	this->addFace(faces[4], lid, offset, low, high);
}

void Map::addFace(BlockFace &face, Quad<Vector3> &quad, Vector3 &offset, Vector2 &low, Vector2 &high)
{
	if(face.tile_number <= 0) return;

	std::vector<Vertex> vertices(4);
	vertices[0].coord = offset + quad.tl;
	vertices[1].coord = offset + quad.tr;
	vertices[2].coord = offset + quad.bl;
	vertices[3].coord = offset + quad.br;

	// We need our texture set to GL_WRAP_AROUND for this to work
	// Obviously, pixels from the other side are leaking around when filtering
	// So this offset tries to eleminate that - maybe fix the matrix to avoid this
	// (Still happens on the mipmapped, distance tile textures)
	// Anisotropic filtering helped, too.

	vertices[0].texcoord = Vector2(0.01, 0.01);
	vertices[1].texcoord = Vector2(0.99, 0.01);
	vertices[2].texcoord = Vector2(0.99, 0.99);
	vertices[3].texcoord = Vector2(0.01, 0.99);

	// fix this - coord-low/high should not affect the texture coords
	// (gta2 doesnt do this either, so we need to do some math magic)
	/*vertices[0].texcoord = Vector2(0.01 * low.x,  0.01 * low.y);
	vertices[1].texcoord = Vector2(0.99 * high.x, 0.01 * low.y);
	vertices[2].texcoord = Vector2(0.99 * high.x, 0.99 * high.y);
	vertices[3].texcoord = Vector2(0.01 * low.x,  0.99 * high.y);*/

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