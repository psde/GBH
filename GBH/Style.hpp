#ifndef GTA2STYLE_HPP
#define GTA2STYLE_HPP

#include "main.hpp"
#include "Gta2Formats.hpp"
#include "Filereader.hpp"

struct PaletteIndex {
	short* phys_palette;
};


class Style
{
private:
	tile tiles[1024];
	tile tiles_flat[1024];
	short* pal_index; // 16384

	PaletteIndex _palette_index;


	PhysPalette* phys_palette[4096];
	int max_phys_palettes;
	PaletteBase* palette_base;
	int* _palette_data;

public:
	bool loadStyle(const char* style);

	int getPaletteValue(int, short);

	unsigned getTexture(int texture, bool flat);
};

#endif