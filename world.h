#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include "chunk.h"
#include "globalSettings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class World
{
public:
	World();
	~World();
	void update(glm::vec2 playerPos, float dt);
	Block* getBlockAtAndHandelNotLoaded(int x, int y, int z, int chunkNumber, Block* block);

private:
	void loadChunks();
	Chunk* loadedChunks[LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH];
	glm::vec3 playerPos;
};

#endif