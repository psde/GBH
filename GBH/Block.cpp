#include "Block.hpp"

Block::Block(BlockInfo blck){
	this->block_info = blck;

	this->groundType = this->block_info.slope_type & 3;
	this->slopeType = (this->block_info.slope_type & 252) >> 2;

    this->faces[0] = Block::getBlockFace(this->block_info.lid);
    this->faces[1] = Block::getBlockFace(this->block_info.top);
    this->faces[2] = Block::getBlockFace(this->block_info.right);
    this->faces[3] = Block::getBlockFace(this->block_info.bottom);
    this->faces[4] = Block::getBlockFace(this->block_info.left);
}

Block::~Block(){
	for(int i=0;i<=4;i++)
		delete this->faces[i];
}

BlockFace* Block::getBlockFace(int bitmap)
{
        BlockFace* temp = new BlockFace();

        temp->tile_number = bitmap & 1023;
        temp->lightning = (bitmap & 1536) >> 10;
        temp->wall = (bitmap & 1024) >> 9;
        temp->bullet_wall = (bitmap & 2048) >> 10;
        temp->flat = (bitmap & 4096) >> 11;
        temp->flip = (bitmap & 8192) >> 12;
        temp->rotation_code = (bitmap & 49152) >> 14;

        return temp;
}

bool Block::isZero(){
	if(	this->block_info.arrows == 0 && this->block_info.bottom == 0 && this->block_info.left == 0 &&
		this->block_info.lid == 0 && this->block_info.right == 0 && this->block_info.slope_type == 0 &&
		this->block_info.top == 0)
		return true;

	return false;
}

void Block::draw(int x, int y, int z, Style* style)
{
	y = y*-1;
	
	if(this->slopeType > 0 && this->groundType != 0) return;

	// temp. block drawing
	// lid
	if(this->faces[0]->tile_number > 0)
	{
		glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[0]->tile_number, this->faces[0]->flat));
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x + 0.0f, y + 0.0f, z + 1.0f);
	
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x + 1.0f, y + 0.0f, z + 1.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
	
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x + 0.0f, y + 1.0f, z + 1.0f);
		glEnd();
	}
	
        // top
	if(this->faces[1]->tile_number > 0)
	{
		glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[1]->tile_number, this->faces[1]->flat));
	        glBegin(GL_QUADS);
        	        glTexCoord2f(0.0f, 1.0f);
                	glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f);

	                glTexCoord2f(1.0f, 1.0f);
        	        glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f);

                	glTexCoord2f(1.0f, 0.0f);
                	glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);

                	glTexCoord2f(0.0f, 0.0f);
                	glVertex3f(x + 0.0f, y + 1.0f, z + 1.0f);
		glEnd();
	}

	// bottom
	if(this->faces[3]->tile_number  > 0)
	{	
		glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[3]->tile_number, this->faces[3]->flat));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x + 1.0f, y + 0.0f, z + 1.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x + 0.0f, y + 0.0f, z + 1.0f);
		glEnd();
	}

	// left
	if(this->faces[4]->tile_number > 0)
	{
        	glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[4]->tile_number, this->faces[4]->flat));
	        glBegin(GL_QUADS);
        	        glTexCoord2f(0.0f, 1.0f);
	                glVertex3f(x + 0.0f, y + 0.0f, z + 0.0f);
	
	                glTexCoord2f(1.0f, 1.0f);
                	glVertex3f(x + 0.0f, y + 1.0f, z + 0.0f);

        	        glTexCoord2f(1.0f, 0.0f);
	                glVertex3f(x + 0.0f, y + 1.0f, z + 1.0f);

                	glTexCoord2f(0.0f, 0.0f);
        	        glVertex3f(x + 0.0f, y + 0.0f, z + 1.0f);
	        glEnd();
	}

        // right
	if(this->faces[2]->tile_number > 0)
	{
        	glBindTexture(GL_TEXTURE_2D, style->getTexture(this->faces[2]->tile_number, this->faces[2]->flat));
       	 	glBegin(GL_QUADS);
                	glTexCoord2f(0.0f, 1.0f);
                	glVertex3f(x + 1.0f, y + 0.0f, z + 0.0f);

	                glTexCoord2f(1.0f, 1.0f);
        	        glVertex3f(x + 1.0f, y + 1.0f, z + 0.0f);

                	glTexCoord2f(1.0f, 0.0f);
	                glVertex3f(x + 1.0f, y + 1.0f, z + 1.0f);
	
        	        glTexCoord2f(0.0f, 0.0f);
                	glVertex3f(x + 1.0f, y + 0.0f, z + 1.0f);
        	glEnd();
	}

}
