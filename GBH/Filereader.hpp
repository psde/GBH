#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include "main.hpp"
#include "Gta2Formats.hpp"

class Filereader{
	private:
		int size;
		char* offset;
		char* map_data;

	public:
		FileHeader* file_header;

		Filereader(const char* filename);
		~Filereader();

		Chunk* getNextChunk();
};

#endif
