#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "chunk.h"
#include "block.h"
#include "globalSettings.h"
#include "world.h"


Chunk::Chunk(){
}

Chunk::Chunk(int worldZ, int worldX, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues, std::vector<float> const& TerrainNoiseValues2, std::vector<float> const& TreeNoiseValues)
{
    this->wx = worldX;
    this->wz = worldZ;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(worldX, -101.0f, worldZ));
    this->chunkIndex = chunkIndex;
	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int y = 0; y < CHUNKHEIGHT; y++)
		{
			for (int z = 0; z < CHUNKWIDTH; z++)
			{
                int blockI = z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x;
				blocks[blockI] = GenorateBlock(x, y, z, (TerrainNoiseValues[z*CHUNKWIDTH+x] + 1) * 0.5f, (TerrainNoiseValues2[z * CHUNKWIDTH + x] + 1) * 0.5f, (TreeNoiseValues[z * CHUNKWIDTH + x] + 1) * 0.5f);
			}
		}
	}
	createTrees();
}

void Chunk::setup() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
}

void Chunk::updateChunkNumber(uint16_t chunkNum)
{
	chunkIndex = chunkNum;
}

Chunk::~Chunk()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Chunk::createMesh() {
	while(locked){}
	locked = true;
	vertices.clear();
	vertices.reserve(180000);
	triangles.clear();
	vertices.reserve(50000);
	for (int i = 0; i < CHUNKWIDTH * CHUNKWIDTH * CHUNKHEIGHT; i++)
	{
		blocks[i].addGemometry(vertices,triangles,chunkIndex);
	}
	readyToBindBuffers = true;
}

void Chunk::bindBuffers() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangles.size(), &triangles[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	shouldDraw = true;
	readyToBindBuffers = false;
	locked = false;
}

void Chunk::reLoadChunk(int worldZ, int worldX, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues, std::vector<float> const& TerrainNoiseValues2, std::vector<float> const& TreeNoiseValues)
{
	this->shouldDraw = false;
	this->wx = worldX;
	this->wz = worldZ;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(worldX, -101.0f, worldZ));
	this->chunkIndex = chunkIndex;
	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int y = 0; y < CHUNKHEIGHT; y++)
		{
			for (int z = 0; z < CHUNKWIDTH; z++)
			{
				int blockI = z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x;
				blocks[blockI] = GenorateBlock(x, y, z, (TerrainNoiseValues[z * CHUNKWIDTH + x] + 1) * 0.5f, (TerrainNoiseValues2[z * CHUNKWIDTH + x] + 1) * 0.5f, (TreeNoiseValues[z * CHUNKWIDTH + x] + 1) * 0.5f);
			}
		}
	}
	createTrees();
}

void Chunk::createTrees() {
	for (size_t i = 0; i < treePoses.size(); i++)
	{
		glm::vec3 treePos = treePoses[i];
		for (int x = -2;  x <= 2;  x++)
		{
			for (int z = -2;  z <= 2;  z++)
			{
				int y = 6;
				blocks[((int)treePos.z + z) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + y) * CHUNKWIDTH + (int)treePos.x + x] = Block(treePos.x + x, treePos.y + y, treePos.z + z, LEAF);
			}
		}
		for (int x = -3; x <= 3; x++)
		{
			for (int z = -3; z <= 3; z++)
			{
				for (int y = 4; y <= 5; y++)
				{
					blocks[((int)treePos.z + z) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + y) * CHUNKWIDTH + (int)treePos.x + x] = Block(treePos.x + x, treePos.y + y, treePos.z + z, LEAF);
				}
			}
		}
		for (int x = -2; x <= 2; x++)
		{
			for (int z = -2; z <= 2; z++)
			{
				int y = 3;
				blocks[((int)treePos.z + z) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + y) * CHUNKWIDTH + (int)treePos.x + x] = Block(treePos.x + x, treePos.y + y, treePos.z + z, LEAF);
			}
		}
		blocks[((int)treePos.z + 0) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + 0) * CHUNKWIDTH + (int)treePos.x + 0] = Block(treePos.x, treePos.y, treePos.z, LOG);
		blocks[((int)treePos.z + 0) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + 1) * CHUNKWIDTH + (int)treePos.x + 0] = Block(treePos.x + 0, treePos.y + 1, treePos.z + 0, LOG);
		blocks[((int)treePos.z + 0) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + 2) * CHUNKWIDTH + (int)treePos.x + 0] = Block(treePos.x + 0, treePos.y + 2, treePos.z + 0, LOG);
		blocks[((int)treePos.z + 0) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + 3) * CHUNKWIDTH + (int)treePos.x + 0] = Block(treePos.x + 0, treePos.y + 3, treePos.z + 0, LOG);
		blocks[((int)treePos.z + 0) * CHUNKHEIGHT * CHUNKWIDTH + ((int)treePos.y + 4) * CHUNKWIDTH + (int)treePos.x + 0] = Block(treePos.x + 0, treePos.y + 4, treePos.z + 0, LOG);
	}
	treePoses.clear();
}

void Chunk::placeBlockAt(int x, int y, int z, int block)
{
	blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].blockType = block;
	createMesh();
	if (x == CHUNKWIDTH - 1)
		world.reloadMesh(chunkIndex + 1);
	else if (x == 0)
		world.reloadMesh(chunkIndex - 1);
	if (z == CHUNKWIDTH - 1)
		world.reloadMesh(chunkIndex + LOADEDCHUNKWIDTH);
	else if (z == 0)
		world.reloadMesh(chunkIndex - LOADEDCHUNKWIDTH);
}

void Chunk::update(float dt)
{
	if (readyToBindBuffers) bindBuffers();
	if (!shouldDraw) return;
	shaders[blockShader].setMat4("model", model);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
}

Block Chunk::getBlockAt(uint8_t x, uint8_t y, uint8_t z)
{
    return blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x];
}

void Chunk::breakBlockAt(uint8_t x, uint8_t y, uint8_t z)
{
	blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].blockType = -1;
	createMesh();
	if (x == CHUNKWIDTH - 1)
		world.reloadMesh(chunkIndex + 1);
	else if (x == 0)
		world.reloadMesh(chunkIndex - 1);
	if (z == CHUNKWIDTH - 1)
		world.reloadMesh(chunkIndex + LOADEDCHUNKWIDTH);
	else if (z == 0)
		world.reloadMesh(chunkIndex - LOADEDCHUNKWIDTH);
}

Block Chunk::GenorateBlock(uint8_t x, uint8_t y, uint8_t z, float noiseValue, float noiseValue2, float TreeNoiseValue) {
	float noiseValueCombined = noiseValue * 0.75f + noiseValue2 * 0.25f;
	if (y > 80 + noiseValueCombined * 25) {
		if (y <= 86)
			return Block(x,y,z,WATER);
		if (y <= 88)
			return Block(x, y, z, AIR);
		if (y <= 81 + noiseValueCombined * 25 && TreeNoiseValue >= 0.9654f && x < 13 && x > 3 && z < 13 && z > 3) {
			treePoses.push_back(glm::vec3(x,y,z));
			return Block(x, y, z, LOG);
		}
		return Block(x, y, z, AIR);
	}
	if (y > 79 + noiseValueCombined * 25) {
		if (y <= 87)
			return Block(x, y, z, SAND);
		return Block(x, y, z, GRASS);
	}
	if (y > 72 + noiseValueCombined * 25)
		return Block(x, y, z, DIRT);
	return Block(x, y, z, STONE);
}
