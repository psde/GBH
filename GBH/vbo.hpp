#ifndef VBO_HPP
#define VBO_HPP

#include "include.hpp"

class VBO
{
	private:
		GLuint buffer;
		long dataSize;

	public:
		VBO()
		{
			glGenBuffers(1, &this->buffer);
			this->dataSize = -1;
		}

		~VBO()
		{
			glDeleteBuffers(1, &this->buffer);
		}

		void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
		}

		long getDataSize() { return this->dataSize; }

		template<typename T> 
			void update(const std::vector<T> &data)
			{
				this->bind();
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), &data[0], GL_STATIC_DRAW);
				this->dataSize = sizeof(T) * data.size();
			}
};

#endif