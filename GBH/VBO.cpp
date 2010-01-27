#include "include.hpp"

VBO::VBO()
{
	glGenBuffers(1, &this->buffer);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &this->buffer);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
}