#include "chunk.h"
#include "globalSettings.h"
Chunk::Chunk(){}
Chunk::Chunk(int worldZ, int worldX)
{
	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int y = 0; y < CHUNKHEIGHT; y++)
		{
			for (int z = 0; z < CHUNKWIDTH; z++)
			{
				blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x] = GenorateBlock(x, y, z);
			}
		}
	}
}

void Chunk::createMesh() {
	vertices.clear();
	triangles.clear();
	uvs.clear();
	for (int i = 0; i < CHUNKWIDTH * CHUNKHEIGHT; i++)
	{
		blocks[i].addGemometry(vertices,triangles,uvs);
	}
	// send to gpu
}

Block Chunk::GenorateBlock(int x, int y, int z) {
	return Block(x, y, z, 1);
}
