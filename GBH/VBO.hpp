#ifndef VBO_HPP
#define VBO_HPP

#include "include.hpp"

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