#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include "chunk.h"
#include "globalSettings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FastNoise/FastNoise.h>
class World
{
public:
	World();
	~World();
	void setup();
	void update(glm::vec2 playerPos, float dt);
	Block* getBlockAt(int x, int y, int z, int chunkNumber);

private:
	void loadChunks();
	Chunk* loadedChunks[LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH];
	int seed;
	glm::vec3 playerPos;
	glm::vec3 centerChunkPos;
	FastNoise::SmartNode<FastNoise::Simplex> grassTerrain;
};

#endif