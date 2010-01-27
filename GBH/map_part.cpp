#include "include.hpp"

MapPart::MapPart()
	: lastModified(Gosu::milliseconds()), lastUpdate(0)
{ 
	this->coordVbo.reset(new VBO);
	this->texcoordVbo.reset(new VBO);
}

void MapPart::pushVertex(Vertex &vert)
{
	this->coord.push_back(vert.coord);
	this->texcoord.push_back(vert.texcoord);

	this->lastModified = Gosu::milliseconds();
}

void MapPart::draw()
{
	if(this->lastModified > this->lastUpdate)
	{
		this->coordVbo->update(this->coord);
		this->texcoordVbo->update(this->texcoord);
		this->lastUpdate = Gosu::milliseconds();
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	this->coordVbo->bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	this->texcoordVbo->bind();
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawArrays(GL_QUADS, 0, coord.size());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}