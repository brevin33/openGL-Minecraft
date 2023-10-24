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
	Chunk(int x, int z, uint8_t chunkIndex, std::vector<float>& TerrainNoiseValues);
	~Chunk();
	void createMesh();
	void update(float dt);
	Block* getBlockAt(uint8_t x, uint8_t y, uint8_t z);

private:
	Block GenorateBlock(uint8_t x, uint8_t y, uint8_t z, float NoiseValue);
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glm::mat4 model, view, projection;
	uint8_t chunkIndex;
	int wx, wz;
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;
	Block blocks[CHUNKWIDTH*CHUNKHEIGHT*CHUNKWIDTH];
};

#endif