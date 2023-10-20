#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
class Block
{
public:
	Block();
	Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType, uint16_t chunkNumber);
	~Block();
	void addGemometry(std::vector<float> &verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs);
private:
	int8_t getLeftBlockType(uint8_t x, uint8_t z);
	int8_t getRightBlockType(uint8_t x, uint8_t z);
	int8_t getTopBlockType(uint8_t x, uint8_t z);
	int8_t getBottomBlockType(uint8_t x, uint8_t z);
	int8_t getFrontBlockType(uint8_t x, uint8_t z);
	int8_t getBackBlockType(uint8_t x, uint8_t z);

	int8_t blockType;
	uint8_t y;
	char xz;
	uint16_t chunkNumber;
};


#endif