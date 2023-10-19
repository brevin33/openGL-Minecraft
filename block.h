#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
class Block
{
public:
	Block();
	Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType, uint8_t chunkNumber);
	~Block();
	void addGemometry(std::vector<float> &verts, std::vector<int>& triangles, std::vector<float>& uvs);
private:
	int8_t getLeftBlockType();
	int8_t getRightBlockType();
	int8_t getTopBlockType();
	int8_t getBottomBlockType();
	int8_t getFrontBlockType();
	int8_t getBackBlockType();

	int8_t blockType;
	uint8_t x, y, z;
	uint8_t chunkNumber;
};


#endif