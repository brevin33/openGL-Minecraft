#include "world.h"
#include "globalSettings.h"
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
World::World()
{
	grassTerrain = FastNoise::New<FastNoise::Simplex>();
	seed = time(NULL);
	centerChunkPos = glm::vec3(0, 0, 0);
	for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			createNewChunk(x, z);
		}
	}
}

World::~World()
{
}

void World::update(float dt)
{
	blockTexture.bind();
	updateLoadedChunks();
	for (size_t i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->update(dt);
	}
}

void World::setup() {
	for (size_t i = 0; i < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH; i++)
	{
		loadedChunks[i]->setup();
		loadedChunks[i]->createMesh();
	}
}

void World::updateChunkIndex() {
	for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x]->updateChunkNumber(z * LOADEDCHUNKWIDTH + x);
		}
	}
}

void World::remeshChunks()
{
	for (const auto& elem : chunksToRemesh) {
		//std::thread thread_obj(&Chunk::createMesh, loadedChunks[elem]);
		//thread_obj.detach();
		loadedChunks[elem]->createMesh();
	}
	chunksToRemesh.clear();
}

void World::updateLoadedChunks() {
	glm::vec3 oldCenterChunkPos = centerChunkPos;
	centerChunkPos = glm::floor(playerPos / (float)CHUNKWIDTH);
	if (oldCenterChunkPos.x < centerChunkPos.x)
		moveCenterChunkRight();
	else if (oldCenterChunkPos.x > centerChunkPos.x)
		moveCenterChunkLeft();
	if (oldCenterChunkPos.z < centerChunkPos.z)
		moveCenterChunkForward();
	else if (oldCenterChunkPos.z > centerChunkPos.z)
 		moveCenterChunkBack();
	updateChunkIndex();
	remeshChunks();
}

Block World::getBlockFromWorldPos(int x, int y, int z)
{
	glm::vec3 playerChunkPos = glm::floor(glm::vec3(x,y,z) / (float)CHUNKWIDTH) - centerChunkPos + glm::vec3(LOADEDCHUNKWIDTH/2,0, LOADEDCHUNKWIDTH/2);
	int blockX = x - CHUNKWIDTH * (floor(x / (float)CHUNKWIDTH));
	int blockY = y + 101;
	int blockZ = z - CHUNKWIDTH * (floor(z/(float)CHUNKWIDTH));
	return getBlockAt(blockX,blockY,blockZ, playerChunkPos.x + playerChunkPos.z * LOADEDCHUNKWIDTH);
}
void World::removeBlockFromWorldPos(int x, int y, int z)
{
	glm::vec3 playerChunkPos = glm::floor(glm::vec3(x, y, z) / (float)CHUNKWIDTH) - centerChunkPos + glm::vec3(LOADEDCHUNKWIDTH / 2, 0, LOADEDCHUNKWIDTH / 2);
	int blockX = x - CHUNKWIDTH * (floor(x / (float)CHUNKWIDTH));
	int blockY = y + 101;
	int blockZ = z - CHUNKWIDTH * (floor(z / (float)CHUNKWIDTH));
	loadedChunks[(int)playerChunkPos.x + (int)playerChunkPos.z * LOADEDCHUNKWIDTH]->breakBlockAt(blockX, blockY, blockZ);
}

void World::reloadMesh(int chunkNumber)
{
	loadedChunks[chunkNumber]->createMesh();
}

void World::placeBlockFromWorldPos(int x, int y, int z, int block)
{
	glm::vec3 playerChunkPos = glm::floor(glm::vec3(x, y, z) / (float)CHUNKWIDTH) - centerChunkPos + glm::vec3(LOADEDCHUNKWIDTH / 2, 0, LOADEDCHUNKWIDTH / 2);
	int blockX = x - CHUNKWIDTH * (floor(x / (float)CHUNKWIDTH));
	int blockY = y + 101;
	int blockZ = z - CHUNKWIDTH * (floor(z / (float)CHUNKWIDTH));
	loadedChunks[(int)playerChunkPos.x + (int)playerChunkPos.z * LOADEDCHUNKWIDTH]->placeBlockAt(blockX, blockY, blockZ,block);
}


void World::moveCenterChunkForward()
{
	Chunk* delChunks[LOADEDCHUNKWIDTH];
	for (size_t z = 0; z < 1; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			delChunks[x] = loadedChunks[z * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = loadedChunks[(z + 1) * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t z = LOADEDCHUNKWIDTH - 1; z < LOADEDCHUNKWIDTH; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			overrideNewChunk(x, z, delChunks[x]);
		}
	}
	for (size_t z = LOADEDCHUNKWIDTH - 2; z < LOADEDCHUNKWIDTH; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			chunksToRemesh.insert(z * LOADEDCHUNKWIDTH + x);
		}
	}
}

void World::moveCenterChunkBack()
{
	Chunk* delChunks[LOADEDCHUNKWIDTH];
	for (size_t z = LOADEDCHUNKWIDTH - 1; z < LOADEDCHUNKWIDTH; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			// ------------------------------------------------------------------------------------- save to file when added
			delChunks[x] = loadedChunks[z * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t z = LOADEDCHUNKWIDTH - 1; z > 0; z--)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = loadedChunks[(z-1) * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t z = 0; z < 1; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			overrideNewChunk(x, z, delChunks[x]);
		}
	}
	for (size_t z = 0; z < 2; z++)
	{
		for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
		{
			chunksToRemesh.insert(z * LOADEDCHUNKWIDTH + x);
		}
	}
}
			
void World::moveCenterChunkRight()
{
	Chunk* delChunks[LOADEDCHUNKWIDTH];
	for (size_t x = 0; x < 1; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			delChunks[z] = loadedChunks[z * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t x = 0; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = loadedChunks[z * LOADEDCHUNKWIDTH + x + 1];
		}
	}
	for (size_t x = LOADEDCHUNKWIDTH - 1; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			overrideNewChunk(x, z, delChunks[z]);
		}
	}
	for (size_t x = LOADEDCHUNKWIDTH-2; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			chunksToRemesh.insert(z * LOADEDCHUNKWIDTH + x);
		}
	}
}

void World::moveCenterChunkLeft()
{
	Chunk* delChunks[LOADEDCHUNKWIDTH];
	for (size_t x = LOADEDCHUNKWIDTH - 1; x < LOADEDCHUNKWIDTH; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			// ------------------------------------------------------------------------------------- save to file when added
			delChunks[z] = loadedChunks[z * LOADEDCHUNKWIDTH + x];
		}
	}
	for (size_t x = LOADEDCHUNKWIDTH - 1; x > 0; x--)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			loadedChunks[z * LOADEDCHUNKWIDTH + x] = loadedChunks[z * LOADEDCHUNKWIDTH + x - 1];
		}
	}
	for (size_t x = 0; x < 1; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			overrideNewChunk(x, z, delChunks[z]);
		}
	}
	for (size_t x = 0; x < 2; x++)
	{
		for (size_t z = 0; z < LOADEDCHUNKWIDTH; z++)
		{
			chunksToRemesh.insert(z * LOADEDCHUNKWIDTH + x);
		}
	}
}


Block World::getBlockAt(int x, int y, int z, int chunkNumber)
{
	return loadedChunks[chunkNumber]->getBlockAt(x,y,z);
}

void World::createNewChunk(int x, int z)
{
	std::vector<float> noiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.018f, seed);
	loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk((z + centerChunkPos.z - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, (x + centerChunkPos.x - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, noiseOutput);
}

void World::overrideNewChunk(int x, int z, Chunk* chunk)
{
	std::vector<float> noiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.018f, seed);
	chunk->reLoadChunk((z + centerChunkPos.z - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, (x + centerChunkPos.x - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, noiseOutput);
	loadedChunks[z * LOADEDCHUNKWIDTH + x] = chunk;
}

void World::loadChunks()
{
}
