#include "world.h"
#include "globalSettings.h"
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
World::World()
{
	grassTerrain = FastNoise::New<FastNoise::Simplex>();
	playerPos = glm::vec3(0,0,0);
	seed = time(NULL);
	centerChunkPos = glm::vec3(0, 0, 0);
	for (int x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (int z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			std::vector<float> noiseOutput(16 * 16);
			grassTerrain->GenUniformGrid2D(noiseOutput.data(), x* CHUNKWIDTH, z* CHUNKWIDTH, 16, 16, 0.018f, seed);
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk((z + centerChunkPos.z - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, (x + centerChunkPos.x - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, noiseOutput);
		}
	}
}

World::~World()
{
}

void World::setup() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
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
