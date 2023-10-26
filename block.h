#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
#include "globalSettings.h"
struct ChunkBlock;
class Block;


class Block
{
public:
	Block();
	Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType);
	~Block();
	void addGemometry(std::vector<float> &verts, std::vector<unsigned int>& triangles, uint16_t chunkNumber);
	ChunkBlock getLeftBlock(uint16_t chunkNumber);
	ChunkBlock getRightBlock(uint16_t chunkNumber);
	ChunkBlock getTopBlock(uint16_t chunkNumber);
	ChunkBlock getBottomBlock(uint16_t chunkNumber);
	ChunkBlock getFrontBlock(uint16_t chunkNumber);
	ChunkBlock getBackBlock(uint16_t chunkNumber);
private:

public:
	int8_t blockType;

private:
	char xz;
	uint8_t y;
};

struct ChunkBlock {
	Block block;
	uint16_t chunkNumber;
};

#endif