#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
class Block
{
public:
	Block();
	Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType, uint16_t chunkNumber);
	~Block();
	void addGemometry(std::vector<float> &verts, std::vector<unsigned int>& triangles);
	Block* getLeftBlock(uint8_t x, uint8_t z);
	Block* getRightBlock(uint8_t x, uint8_t z);
	Block* getTopBlock(uint8_t x, uint8_t z);
	Block* getBottomBlock(uint8_t x, uint8_t z);
	Block* getFrontBlock(uint8_t x, uint8_t z);
	Block* getBackBlock(uint8_t x, uint8_t z);
	Block* getLeftBlock();
	Block* getRightBlock();
	Block* getTopBlock();
	Block* getBottomBlock();
	Block* getFrontBlock();
	Block* getBackBlock();
private:

public:
	int8_t blockType;
	uint16_t chunkNumber;

private:
	char xz;
	uint8_t y;
};


#endif