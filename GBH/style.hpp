#ifndef GTA2STYLE_HPP
#define GTA2STYLE_HPP

#include "include.hpp"

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

	int textureCount;

public:
	Style(const char* style);
	~Style();

	int getPaletteValue(int, short);

	unsigned getTexture(int texture, short flat);
	unsigned getTexture(int texture, bool flat);
};

#endif