#include "Block.hpp"

/*VBO_TexCoord* manipulateTexCoord(VBO_TexCoord *texcoord, int size, int rotation, int flip){
	if(size != 4) throw std::runtime_error("We only support faces with 4 coordinates");

	VBO_TexCoord* ret = (VBO_TexCoord *)(malloc(sizeof(VBO_TexCoord)*size));

	for(int i=0;i<size;i++){
		ret[i] = texcoord[i];
	}


	if(flip != 0){
		for(int i=0;i<size;i++){
			ret[i].u = ret[i].u * -1;
		}
	}
	return ret;
}
*/
_block_face* _getBlockFace(int foo){
	_block_face* temp = new _block_face();

	temp->tile_number = foo & 1023;
	temp->lightning = (foo & 1536) >> 10;
	temp->wall = (foo & 1024) >> 9;
	temp->bullet_wall = (foo & 2048) >> 10;
	temp->flat = (foo & 4096) >> 11;
	temp->flip = (foo & 8192) >> 12;
	temp->rotation_code = (foo & 49152) >> 14;

	return temp;
}

BlockReader *Block::block_reader = NULL;

Block::Block(BlockInfo blck){
	if(Block::block_reader == NULL){
		Block::block_reader = new BlockReader();
		Block::block_reader->readBlocks();
	}
	this->block_info = blck;
	this->numVBOs = 0;

	this->ground_type = this->block_info.slope_type & 3;
	this->slope_type = (this->block_info.slope_type & 252) >> 2;

	this->VBOsBuild = false;
}

Block::~Block(){

}

bool Block::isZero(){
	if(	this->block_info.arrows == 0 && this->block_info.bottom == 0 && this->block_info.left == 0 &&
		this->block_info.lid == 0 && this->block_info.right == 0 && this->block_info.slope_type == 0 &&
		this->block_info.top == 0)
		return true;
	else
		return false;
}

void Block::buildVBOs(int x_offset, int y_offset, int z_offset, Style* style){

	if(VBOsBuild) return;

	this->faces[0] = _getBlockFace(this->block_info.lid);
	this->faces[1] = _getBlockFace(this->block_info.top);
	this->faces[2] = _getBlockFace(this->block_info.right);
	this->faces[3] = _getBlockFace(this->block_info.bottom);
	this->faces[4] = _getBlockFace(this->block_info.left);


/*	VBO_TexCoord* texcoord_org = new VBO_TexCoord[4];

	texcoord_org[0].u = 0;
	texcoord_org[0].v = 0;

	texcoord_org[1].u = 1;
	texcoord_org[1].v = 0;

	texcoord_org[2].u = 0;
	texcoord_org[2].v = 1;

	texcoord_org[3].u = 1;
	texcoord_org[3].v = 1;*/


	for(int i=0;i<=4;i++){

		int tile_number = this->faces[i]->tile_number;
		int rotation = this->faces[i]->rotation_code;
		int flip = this->faces[i]->flip;
		int flat = this->faces[i]->flat;

		int x_offset2, y_offset2, z_offset2;
		x_offset2 = y_offset2 = z_offset2 = 0;

		if(tile_number > 0 && i == 1){
			if(this->faces[3]->flat && !this->faces[1]->flat && this->faces[3]->tile_number > 0) y_offset2 = 1;
		}

		if(tile_number > 0 && i == 3){
			if(this->faces[1]->flat && !this->faces[3]->flat && this->faces[1]->tile_number > 0) y_offset2 = -1;
		}

		if(tile_number > 0 && i == 2){
			if(this->faces[4]->flat && !this->faces[2]->flat && this->faces[4]->tile_number > 0) x_offset2 = -1;
		}

		if(tile_number > 0 && i == 4){
			if(this->faces[2]->flat && !this->faces[4]->flat && this->faces[2]->tile_number > 0) x_offset2 = 1;
		}


		if(tile_number > 0 && block_reader->getCount(this->slope_type,i) > 0){

			/*VBO_TexCoord *texcoord = block_reader->getTexCoord(this->slope_type,i);
			texcoord = manipulateTexCoord(texcoord,block_reader->getCount(this->slope_type,i),rotation,flip);*/
/*
			if(flip != 0){
				for(int s=0;s<block_reader->getCount(this->slope_type,i);s++){
					texcoord[s].u = texcoord[s].u * -1;
				}
			}*/
/*
			float *pos = (float*)(malloc(sizeof(float)*3));
			pos[0] = (float)x_offset+x_offset2;
			pos[1] = (float)y_offset+y_offset2;
			pos[2] = (float)z_offset+z_offset2;

			VBO* temp = new VBO(block_reader->getVertices(this->slope_type,i,x_offset+x_offset2,y_offset+y_offset2,z_offset+z_offset2), texcoord, style->getTexture(tile_number, flat), (bool)(this->faces[i]->flat), pos);

			temp->Vertex_Count = block_reader->getCount(this->slope_type,i);

			temp->rotate = rotation;
			temp->flip = flip;

			temp->build();*/

			//this->vbos[this->numVBOs] = temp;

			this->numVBOs++;
		}
	}
	this->VBOsBuild = true;
}

/*int Block::getNumVBOs(){
	return this->numVBOs;
}

VBO* Block::getVBO(int n){
	return this->vbos[n];
}*/

void Block::draw(int x, int y, int z, Style* style)
{
	/*this->faces[0] = _getBlockFace(this->block_info.lid);
	this->faces[1] = _getBlockFace(this->block_info.top);
	this->faces[2] = _getBlockFace(this->block_info.right);
	this->faces[3] = _getBlockFace(this->block_info.bottom);
	this->faces[4] = _getBlockFace(this->block_info.left);*/

	//std::cout << style->getTexture(this->faces[0]->tile_number, this->faces[0]->flat) << std::endl; 
	glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[0]->tile_number, this->faces[0]->flat));
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f);
	glEnd();
}