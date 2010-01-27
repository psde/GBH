#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include "include.hpp"

class Filereader{
	private:
		int size;
		char* offset;
		char* mapData;

	public:
		FileHeader* file_header;

		Filereader(const char* filename);
		~Filereader();

		Chunk* getNextChunk();
};

#endif
