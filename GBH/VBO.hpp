#ifndef VBO_HPP
#define VBO_HPP

#include "main.hpp"


struct Vertex3 {
	float x;
	float y;
	float z;
};

struct Vertex2 {
	float x;
	float y;
};

class VBO
{
	private:
		GLuint buffer;

	public:
		VBO();
		~VBO();

		void bind();
};

#endif