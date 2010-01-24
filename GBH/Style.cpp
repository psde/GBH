#include "Style.hpp"

bool Style::loadStyle(const char* style){

	Filereader* reader = new Filereader(style);
	Chunk* chk;
	Chunk* tile_data;
	Chunk* palette_data;
	
	short* phys_palette;

	while((chk = reader->getNextChunk())){
		char* offset = chk->data;

		if(strncmp(chk->header->type,"PALX",4) == 0) {
			this->pal_index = reinterpret_cast<short*>(offset);
			phys_palette = reinterpret_cast<short*>(offset);
			_palette_index.phys_palette = reinterpret_cast<short*>(offset);


		}

		if(strncmp(chk->header->type,"PALB",4) == 0) {
			this->palette_base = reinterpret_cast<PaletteBase*>(offset);
		}

		if(strncmp(chk->header->type,"PPAL",4) == 0) {
			palette_data = chk;
			_palette_data = reinterpret_cast<int*>(offset);
		}

		if(strncmp(chk->header->type,"TILE",4) == 0) {
			tile_data = chk;
		}

	}

	const int page_size = 256 * 256;
	for(int page_num = 0; page_num < 62; page_num++)
	{
		char* page = tile_data->data + page_size * page_num;

		for(int y = 0; y < 4; y++)
		{
			for(int x = 0; x < 4; x++)
			{
				int tile[64*64];
				const int tile_index = page_num * 16 + y * 4 + x;
				int palette_index = _palette_index.phys_palette[tile_index];

				for(int tile_y = 0; tile_y < 64; tile_y++)
				{
					for(int tile_x = 0; tile_x < 64; tile_x++)
					{
						unsigned char c = page[(y * 64 + tile_y) * 256 + x * 64 + tile_x];
						tile[tile_y * 64 + tile_x] = (c == 0) ? 0x00000000 : (getPaletteValue(palette_index, c) | 0xff000000);
					}
				}
			
				glGenTextures(1, &this->tiles[tile_index].gl_i);
				glBindTexture(GL_TEXTURE_2D, this->tiles[tile_index].gl_i);
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, 1);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_BGRA, GL_UNSIGNED_BYTE, tile);
				//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 64, 64, GL_BGRA, GL_UNSIGNED_BYTE, tile);

				glGenTextures(1, &this->tiles_flat[tile_index].gl_i);
				glBindTexture(GL_TEXTURE_2D, this->tiles_flat[tile_index].gl_i);
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, 1);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_BGRA, GL_UNSIGNED_BYTE, tile);
				//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 64, 64, GL_BGRA, GL_UNSIGNED_BYTE, tile);
			}
		}
	}

	delete[] chk;
	delete[] tile_data;
	delete[] palette_data;

	return true;
}

int Style::getPaletteValue(int palette_index, short color_index){
	int page_start = (palette_index / 64) * 64 * 256 * 4;
	int* page = _palette_data + page_start;
	return page[(palette_index % 64) + color_index * 64];
}


unsigned Style::getTexture(int texture, bool flat){
	flat = true;
	if(flat){
		return this->tiles_flat[texture].gl_i;	
	}else{
		return this->tiles[texture].gl_i;	
	}
};