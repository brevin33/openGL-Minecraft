#include "chunk.h"
#include "block.h"
#include "globalSettings.h"
#include "world.h"


Chunk::Chunk(){

}

Chunk::Chunk(int worldZ, int worldX, uint8_t chunkIndex)
{
    this->wx = worldX;
    this->wz = worldZ;
    this->chunkIndex = chunkIndex;
	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int y = 0; y < CHUNKHEIGHT; y++)
		{
			for (int z = 0; z < CHUNKWIDTH; z++)
			{
                int blockI = z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x;
				blocks[blockI] = GenorateBlock(x, y, z);
			}
		}
	}
}

Chunk::~Chunk()
{
}

void Chunk::createMesh() {
	vertices.clear();
	triangles.clear();
	uvs.clear();
	for (int i = 0; i < CHUNKWIDTH * CHUNKHEIGHT; i++)
	{
        Block b = blocks[i];
		b.addGemometry(vertices,triangles,uvs);
	}
	int test = 1;
	// send to gpu
}

void Chunk::update(float dt)
{
}

Block* Chunk::getBlockAt(uint8_t x, uint8_t y, uint8_t z)
{
    return &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x];
}

Block Chunk::GenorateBlock(uint8_t x, uint8_t y, uint8_t z) {
	return Block(x, y, z, DIRT, chunkIndex);
}
