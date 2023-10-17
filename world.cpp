#include "world.h"

World::World()
{
	playerPos = glm::vec3(0,0,0);
	for (int x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (int z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk(z * CHUNKWIDTH, x * CHUNKWIDTH);
		}
	}
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->giveBlocksNeighbors();
		loadedChunks[i]->createMesh();
	}
}

void World::update(glm::vec2 playerPos, float dt)
{
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->update(dt);
	}
}
