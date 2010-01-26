#include "Filereader.hpp"

Filereader::Filereader(const char* filename){
	ifstream file (filename, ios::in|ios::binary|ios::ate);

	if(!file.is_open()) throw runtime_error("Filereader: file not found");

	file.seekg(0, std::ios::end);
	this->size = file.tellg();
	file.seekg(0, std::ios::beg);

	this->mapData = new char[size];
	file.read(mapData,size);

	file.close();

	this->offset = mapData;

	this->file_header = reinterpret_cast<FileHeader*>(this->offset);
	this->offset += sizeof(FileHeader);

	if(strncmp(this->file_header->name,"GBMP",4) != 0 && strncmp(this->file_header->name,"GBST",4) != 0) throw runtime_error("Filereader: not a GTA2 data file.");
}

Filereader::~Filereader(){
	delete this->mapData;
}

Chunk* Filereader::getNextChunk(){
	Chunk *chk = reinterpret_cast<Chunk*>(malloc(sizeof(Chunk)));
	
	ChunkHeader* chk_header; 
	if(this->offset < this->mapData + this->size){
	
		chk_header = reinterpret_cast<ChunkHeader*>(this->offset);
		this->offset += sizeof(ChunkHeader);
		chk->header = chk_header;
		char* data =reinterpret_cast<char*>(this->offset);
		chk->data = data;

		this->offset += chk_header->size;
		return chk;
	}else{
		/*chk_header = reinterpret_cast<ChunkHeader*>(malloc(sizeof(ChunkHeader)));
		chk_header->size = 0;
		chk->header = chk_header;*/
		return 0;
	}
}