#include "BlockReader.hpp"

BlockReader::BlockReader(){
	for(int i=0;i<=63;i++){
		this->block_structure[i] = new BlockStructure;
		for(int s=0;s<4;s++){
			this->block_structure[i]->hasStuff[s] = false;
			this->block_structure[i]->size[s] = 0;
		}
	}

}

void readstr(FILE *f,char *string){
	do{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

void BlockReader::readBlocks(){

	/*for(int block_number=0;block_number<=63;block_number++){
		stringstream filename;

		filename << "data/blocks/" << block_number << ".block";

		if(fileExists(filename.str())) {

			FILE *file;
			char line[255];
			file = fopen(filename.str().c_str(), "rt");

			readstr(file,line);

			int num_faces;
			sscanf(line, "f: %d\n", &num_faces);

			for(int faces=0;faces<num_faces;faces++){
				readstr(file,line);
				int type;
				sscanf(line, "t: %d\n", &type);

				readstr(file,line);
				int num_vertices;
				sscanf(line, "n: %d\n", &num_vertices);

				VBO_Vert* vert_temp = new VBO_Vert[num_vertices];
				VBO_TexCoord* tex_temp = new VBO_TexCoord[num_vertices];

				for(int vert=0;vert<num_vertices;vert++){

					readstr(file,line);
					sscanf(line, "v: %f %f %f %f %f", &vert_temp[vert].x, &vert_temp[vert].y, &vert_temp[vert].z, &tex_temp[vert].u, &tex_temp[vert].v);
				}

				this->block_structure[block_number]->VBO_Vertices[type] = vert_temp;
				this->block_structure[block_number]->VBO_Tex[type] = tex_temp	;
				this->block_structure[block_number]->hasStuff[type] = true;
				this->block_structure[block_number]->size[type] = num_vertices;

			}
		}
	}*/
}

/*
VBO_Vert *BlockReader::getVertices(int block_number, int vert_number, int x_offset, int y_offset, int z_offset){
	VBO_Vert *vert_org = this->block_structure[block_number]->VBO_Vertices[vert_number];
	VBO_Vert *vert_temp = new VBO_Vert[this->getCount(block_number,vert_number)];

	vert_temp[0].x = (float)x_offset+vert_org[0].x;
	vert_temp[0].y = (float)-y_offset+vert_org[0].y;
	vert_temp[0].z = (float)z_offset+vert_org[0].z;

	vert_temp[1].x = (float)x_offset+vert_org[1].x;
	vert_temp[1].y = (float)-y_offset+vert_org[1].y;
	vert_temp[1].z = (float)z_offset+vert_org[1].z;

	vert_temp[2].x = (float)x_offset+vert_org[2].x;
	vert_temp[2].y = (float)-y_offset+vert_org[2].y;
	vert_temp[2].z = (float)z_offset+vert_org[2].z;

	vert_temp[3].x = (float)x_offset+vert_org[3].x;
	vert_temp[3].y = (float)-y_offset+vert_org[3].y;
	vert_temp[3].z = (float)z_offset+vert_org[3].z;

	return vert_temp;
}

VBO_TexCoord *BlockReader::getTexCoord(int block_number, int vert_number){
	return this->block_structure[block_number]->VBO_Tex[vert_number];
}
*/

int BlockReader::getCount(int block_number, int vert_number){
	return this->block_structure[block_number]->size[vert_number];
}