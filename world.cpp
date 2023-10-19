#include "world.h"
#include "globalSettings.h"
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
World::World()
{
	playerPos = glm::vec3(0,0,0);
	for (int x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (int z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk(z * CHUNKWIDTH, x * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, this);
		}
	}
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->giveBlocksNeighbors();
		loadedChunks[i]->createMesh();
	}
}

World::~World()
{
}

void World::update(glm::vec2 playerPos, float dt)
{
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->update(dt);
	}
}

Block* World::getBlockAtAndHandelNotLoaded(int x, int y, int z, int chunkNumber, Block* block)
{
	return nullptr;
}

void World::loadChunks()
{
}
