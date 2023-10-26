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

Chunk::Chunk(int worldZ, int worldX, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues)
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
				blocks[blockI] = GenorateBlock(x, y, z, (TerrainNoiseValues[z*CHUNKWIDTH+x] + 1) * 0.5f);
			}
		}
	}
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
	vertices.clear();
	triangles.clear();
	for (int i = 0; i < CHUNKWIDTH * CHUNKWIDTH * CHUNKHEIGHT; i++)
	{
		blocks[i].addGemometry(vertices,triangles,chunkIndex);
	}
	readyToBindBuffers = true;
}

void Chunk::bindBuffers() {
	if (!first) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, CHUNKBUFFERSIZE * sizeof(float), &vertices[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0.5f * CHUNKBUFFERSIZE * sizeof(unsigned int), &triangles[0]);
		shouldDraw = true;
		readyToBindBuffers = false;
	}
	else {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, CHUNKBUFFERSIZE * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0.5f * sizeof(unsigned int) * CHUNKBUFFERSIZE, &triangles[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		shouldDraw = true;
		readyToBindBuffers = false;
		first = false;
	}
}

void Chunk::reLoadChunk(int worldZ, int worldX, uint8_t chunkIndex, std::vector<float> const& TerrainNoiseValues)
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
				blocks[blockI] = GenorateBlock(x, y, z, (TerrainNoiseValues[z * CHUNKWIDTH + x] + 1) * 0.5f);
			}
		}
	}
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

Block Chunk::GenorateBlock(uint8_t x, uint8_t y, uint8_t z, float NoiseValue) {
	if(y > 93 + NoiseValue * 7)
		return Block(x, y, z, AIR);
	return Block(x, y, z, DIRT);
}
