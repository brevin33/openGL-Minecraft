#include "world.h"
#include "globalSettings.h"
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>
#include <mutex>
std::mutex chunksToRemeshLock;
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
	for (int i = 0; i < 1; i++)
	{
		std::thread thread_obj(&World::remeshChunks, this);
		thread_obj.detach();
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
	while (true) {
		if (chunksToRemesh.size() == 0) continue;
		chunksToRemeshLock.lock();
		if (chunksToRemesh.size() == 0) {
			chunksToRemeshLock.unlock();
			continue;
		}
		auto elem = *chunksToRemesh.begin();
		chunksToRemesh.erase(chunksToRemesh.begin());
		Chunk* chunk = loadedChunks[elem];
		chunksToRemeshLock.unlock();
		chunk->createMesh();
	}
}

void World::updateLoadedChunks() {
	glm::vec3 oldCenterChunkPos = centerChunkPos;
	centerChunkPos = glm::floor(playerPos / (float)CHUNKWIDTH);
	if (oldCenterChunkPos.x < centerChunkPos.x) {
		chunksToRemeshLock.lock();
		moveCenterChunkRight();
		chunksToRemeshLock.unlock();
	}
	else if (oldCenterChunkPos.x > centerChunkPos.x) {
		chunksToRemeshLock.lock();
		moveCenterChunkLeft();
		chunksToRemeshLock.unlock();
	}
	if (oldCenterChunkPos.z < centerChunkPos.z) {
		chunksToRemeshLock.lock();
		moveCenterChunkForward();
		chunksToRemeshLock.unlock();
	}
	else if (oldCenterChunkPos.z > centerChunkPos.z) {
		chunksToRemeshLock.lock();
		moveCenterChunkBack();
		chunksToRemeshLock.unlock();

	}
	updateChunkIndex();
	//remeshChunks();
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
	loadedChunksLock = true;
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
	loadedChunksLock = false;
}

void World::moveCenterChunkBack()
{
	loadedChunksLock = true;
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
	loadedChunksLock = false;
}
			
void World::moveCenterChunkRight()
{
	loadedChunksLock = true;
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
	loadedChunksLock = false;
}

void World::moveCenterChunkLeft()
{
	loadedChunksLock = true;
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
	loadedChunksLock = false;
}


Block World::getBlockAt(int x, int y, int z, int chunkNumber)
{
	while(loadedChunksLock){}
	return loadedChunks[chunkNumber]->getBlockAt(x,y,z);
}

void World::createNewChunk(int x, int z)
{
	std::vector<float> noiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.003f, seed);
	std::vector<float> noiseOutput2(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput2.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.012f, seed);
	std::vector<float> treeNoiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(treeNoiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 2.0f, seed);
	loadedChunks[z * LOADEDCHUNKWIDTH + x] = new Chunk((z + centerChunkPos.z - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, (x + centerChunkPos.x - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, noiseOutput, noiseOutput2, treeNoiseOutput);
}

void World::overrideNewChunk(int x, int z, Chunk* chunk)
{
	std::vector<float> noiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.003f, seed);
	std::vector<float> noiseOutput2(16 * 16);
	grassTerrain->GenUniformGrid2D(noiseOutput2.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 0.012f, seed);
	std::vector<float> treeNoiseOutput(16 * 16);
	grassTerrain->GenUniformGrid2D(treeNoiseOutput.data(), (centerChunkPos.x + x) * CHUNKWIDTH, (centerChunkPos.z + z) * CHUNKWIDTH, 16, 16, 4.0f, seed);
	chunk->reLoadChunk((z + centerChunkPos.z - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, (x + centerChunkPos.x - LOADEDCHUNKWIDTH / 2) * CHUNKWIDTH, z * LOADEDCHUNKWIDTH + x, noiseOutput, noiseOutput2, treeNoiseOutput);
	loadedChunks[z * LOADEDCHUNKWIDTH + x] = chunk;
}