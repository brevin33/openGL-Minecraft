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
	Chunk(int z, int x, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues, std::vector<float> const& TerrainNoiseValues2, std::vector<float> const& TreeNoiseValues);
	~Chunk();
	void createMesh();
	void update(float dt);
	Block getBlockAt(uint8_t x, uint8_t y, uint8_t z);
	void breakBlockAt(uint8_t x, uint8_t y, uint8_t z);
	uint8_t chunkIndex;
	void setup();
	void updateChunkNumber(uint16_t chunkNum);
	void bindBuffers();
	void reLoadChunk(int x, int z, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues, std::vector<float> const& TerrainNoiseValues2, std::vector<float> const& TreeNoiseValues);
	void placeBlockAt(int x, int y, int z, int block);
private:
	Block GenorateBlock(uint8_t x, uint8_t y, uint8_t z, float NoiseValue, float noiseValue2, float TreeNoiseValue);
	void createTrees();

public:
	bool shouldDraw = false;
	bool readyToBindBuffers = false;
	bool locked = false;
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	std::vector<glm::vec3> treePoses;
	glm::mat4 model;
	int wx, wz;
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;
	Block blocks[CHUNKWIDTH*CHUNKHEIGHT*CHUNKWIDTH];
	bool first = true;
};

#endif