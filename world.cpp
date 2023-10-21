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
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk(z * CHUNKWIDTH, x * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x);
		}
	}
}

World::~World()
{
}

void World::setup() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->createMesh();
	}
}

void World::update(glm::vec2 playerPos, float dt)
{
	blockTexture.bind();
	for (int i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->update(dt);
	}
}

Block* World::getBlockAt(int x, int y, int z, int chunkNumber)
{
	return loadedChunks[chunkNumber]->getBlockAt(x,y,z);
}

void World::loadChunks()
{
}
