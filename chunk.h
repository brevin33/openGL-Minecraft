#ifndef CHUNK_H
#define CHUNK_H
#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Chunk
{
public:
	Chunk();
	Chunk(int x, int z, int chunkIndex, World* w);
	~Chunk();
	void createMesh();
	void giveBlocksNeighbors();
	void update(float dt);

private:
	Block GenorateBlock(int x, int y, int z);

	int chunkIndex;
	int wx, wz;
	World *world;
	std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<int> triangles;
	Block blocks[CHUNKWIDTH*CHUNKHEIGHT*CHUNKWIDTH];
};

#endif