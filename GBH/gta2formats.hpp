#ifndef GTA2FORMAT_HPP
#define GTA2FORMAT_HPP

struct FileHeader {
	char name[4];
	short version;
};

struct ChunkHeader {
	char type[4];
	int size;
};

struct Chunk {
	ChunkHeader* header;
	char* data;
};

struct BlockInfo {
	short left, right, top, bottom, lid;
	char arrows;
	char slope_type;
};

struct CompressedMap {
	int* base;
	int column_words;
	int* columns;
	int num_blocks;
	BlockInfo* blocks;
};

struct ColInfo {
	char height;
	char offset;
	short pad;
	int blockd[8];
};

struct TileAnimation {
	short base;
	char frame_rate;
	char repeat;
	unsigned char anim_length;
	char unused;
	short* tiles; 
};

struct PhysPalette {
	int* palette;
};

struct PhysPalettePage {
	PhysPalette* phys_palette;
};

struct PaletteBase{
	short tile;
	short sprite;
	short car_remap;
	short ped_remap;
	short code_obj_remap;
	short map_obj_remap;
	short user_remap;
	short font_remap;
};

struct tile {
	int data[64][64];
	unsigned gl_i;
};

#endif