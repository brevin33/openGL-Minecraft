#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include "nibble.h"
#include <vector>

Block dummyTransparentBlock(0,0,0,-1,0);
Block dummyNonTransparentBlock(0, 0, 0, 1, 0);

inline int isTransparent(Block* block) {
	return block->blockType >= 0 ? 1 : 0;
}

inline float* ambientOcclusionTopAndBottomFace(Block* block) {
	float occlusionValues[4];
	Block* frontBlock = block->getFrontBlock();
	Block* backBlock = block->getBackBlock();
	Block* leftBlock = block->getLeftBlock();
	Block* rightBlock = block->getRightBlock();
	Block* frontLeftCorrner = frontBlock->getLeftBlock();
	Block* frontRightCorrner = frontBlock->getRightBlock();
	Block* backLeftCorrner = backBlock->getLeftBlock();
	Block* backRightCorrner = backBlock->getRightBlock();
	occlusionValues[0] = isTransparent(backBlock) + isTransparent(leftBlock) + isTransparent(backLeftCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(frontBlock) + isTransparent(leftBlock) + isTransparent(frontLeftCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(backBlock) + isTransparent(rightBlock) + isTransparent(backRightCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(frontBlock) + isTransparent(rightBlock) + isTransparent(frontRightCorrner) * 0.5f;
	return occlusionValues;
}

inline void addTopFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionTopAndBottomFace(block);

	float faceVerts[] = {
		x, y, z, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x, y, z - 1, uvx, uvy2, occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y, z, uvx2, uvy, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y, z - 1, uvx2, uvy2, occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 1, size + 3,
		size, size + 3, size + 2,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}


inline void addBottomFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionTopAndBottomFace(block);
	float faceVerts[] = {
		x, y-1, z, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x, y-1, z - 1, uvx, uvy2, occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y-1, z, uvx2, uvy, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y-1, z - 1, uvx2, uvy2, occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 3, size + 1,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}

inline float* ambientOcclusionLeftAndRightFace(Block* block) {
	float occlusionValues[4];
	Block* frontBlock = block->getFrontBlock();
	Block* backBlock = block->getBackBlock();
	Block* topBlock = block->getTopBlock();
	Block* bottomBlock = block->getBottomBlock();
	Block* frontTopCorrner = frontBlock->getTopBlock();
	Block* frontBottomCorrner = frontBlock->getBottomBlock();
	Block* backTopCorrner = backBlock->getTopBlock();
	Block* backBottomCorrner = backBlock->getBottomBlock();
	occlusionValues[0] = isTransparent(backBlock) + isTransparent(topBlock) + isTransparent(backTopCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(frontBlock) + isTransparent(topBlock) + isTransparent(frontTopCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(frontBlock) + isTransparent(bottomBlock) + isTransparent(frontBottomCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(backBlock) + isTransparent(bottomBlock) + isTransparent(backBottomCorrner) * 0.5f;
	return occlusionValues;
}


inline void addLeftFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionLeftAndRightFace(block);

	float faceVerts[] = {
		x, y, z, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x, y, z - 1, uvx, uvy2, occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x, y - 1, z - 1, uvx2, uvy2, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x, y - 1, z, uvx2, uvy,occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 3, size + 2,
		size, size + 2,size + 1,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}


inline void addRightFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionLeftAndRightFace(block);
	float faceVerts[] = {
		x + 1, y, z, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y, z - 1, uvx, uvy2, occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y - 1, z - 1, uvx2, uvy2, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y - 1, z, uvx2, uvy, occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 2, size + 3,
		size, size + 1,size + 2,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}

inline float* ambientOcclusionFrontAndBackFace(Block* block) {
	float occlusionValues[4];
	Block* leftBlock = block->getLeftBlock();
	Block* rightBlock = block->getRightBlock();
	Block* topBlock = block->getTopBlock();
	Block* bottomBlock = block->getBottomBlock();
	Block* leftTopCorrner = leftBlock->getTopBlock();
	Block* leftBottomCorrner = leftBlock->getBottomBlock();
	Block* rightTopCorrner = rightBlock->getTopBlock();
	Block* rightBottomCorrner = rightBlock->getBottomBlock();
	occlusionValues[0] = isTransparent(leftBlock) + isTransparent(topBlock) + isTransparent(leftTopCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(rightBlock) + isTransparent(topBlock) + isTransparent(rightTopCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(rightBlock) + isTransparent(bottomBlock) + isTransparent(rightBottomCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(leftBlock) + isTransparent(bottomBlock) + isTransparent(leftBottomCorrner) * 0.5f;
	return occlusionValues;
}

inline void addFrontFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionFrontAndBackFace(block);

	float t1 = occlusionValues[0];
	float t2 = occlusionValues[1];
	float t3 = occlusionValues[2];
	float t4 = occlusionValues[3];
	float faceVerts[] = {
		x, y, z - 1, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y, z - 1, uvx, uvy2,occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y - 1, z - 1, uvx2, uvy2, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x, y - 1, z - 1, uvx2, uvy,occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 2, size + 1,
		size, size + 3,size + 2,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}

inline void addBackFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, Block* block) {
	int size = verts.size()/6;
	float uvx = (suvx * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvx2 = ((suvx+1) * SPRITEWIDTH) / TEXTUREATLASWIDTH;
	float uvy = (suvy * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float uvy2 = ((suvy+1) * SPRITEWIDTH) / TEXTUREATLASHEIGHT;
	float* occlusionValues = ambientOcclusionFrontAndBackFace(block);
	float faceVerts[] = {
		x, y, z, uvx, uvy, occlusionValues[0] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y, z, uvx, uvy2, occlusionValues[1] * AMBIENTOCCLUSIONSTRENGTH,
		x + 1, y - 1, z, uvx2, uvy2, occlusionValues[2] * AMBIENTOCCLUSIONSTRENGTH,
		x, y - 1, z, uvx2, uvy, occlusionValues[3] * AMBIENTOCCLUSIONSTRENGTH,
	};
	int faceTriangles[] = {
		size, size + 1, size + 2,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < BLOCKVAOCOUNT; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
	 
}

Block::Block()
{
}

Block::Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType, uint16_t chunkNumber) {
	xz = 0;
	xz = setLowerNibble(xz, x);
	xz = setUpperNibble(xz, z);
	this->y = y;
	this->blockType = blockType;
	this->chunkNumber = chunkNumber;
}

Block::~Block()
{
}


Block* Block::getLeftBlock(uint8_t x, uint8_t z)
{
	if (x - 1 >= 0)
		return world.getBlockAt(x-1,y,z,chunkNumber);
	else if(chunkNumber % LOADEDCHUNKWIDTH != 0)
		return world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkNumber - 1);
	return &dummyNonTransparentBlock;
}

Block* Block::getRightBlock(uint8_t x, uint8_t z)
{
	if (x + 1 < CHUNKWIDTH)
		return world.getBlockAt(x + 1, y, z, chunkNumber);
	else if (chunkNumber % LOADEDCHUNKWIDTH != LOADEDCHUNKWIDTH - 1)
		return world.getBlockAt(0, y, z, chunkNumber + 1);
	return &dummyNonTransparentBlock;
}


Block* Block::getTopBlock(uint8_t x, uint8_t z)
{
	if (y + 1 < CHUNKHEIGHT)
		return world.getBlockAt(x, y + 1, z, chunkNumber);
	return &dummyTransparentBlock;
}


Block* Block::getBottomBlock(uint8_t x, uint8_t z)
{
	if (y - 1 > 0)
		return world.getBlockAt(x, y - 1, z, chunkNumber);
	return &dummyNonTransparentBlock;
}

Block* Block::getFrontBlock(uint8_t x, uint8_t z)
{
	if (z - 1 >= 0)
		return world.getBlockAt(x, y, z - 1, chunkNumber);
	else if (chunkNumber - LOADEDCHUNKWIDTH >= 0)
		return world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkNumber - LOADEDCHUNKWIDTH);
	return &dummyNonTransparentBlock;
}

Block* Block::getBackBlock(uint8_t x, uint8_t z)
{
	if (z + 1 < CHUNKWIDTH)
		return world.getBlockAt(x, y, z + 1, chunkNumber);
	else if (chunkNumber + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH)
		return world.getBlockAt(x, y, 0, chunkNumber + LOADEDCHUNKWIDTH);
	return &dummyNonTransparentBlock;
}

Block* Block::getLeftBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (x - 1 >= 0)
		return world.getBlockAt(x - 1, y, z, chunkNumber);
	else if (chunkNumber % LOADEDCHUNKWIDTH != 0)
		return world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkNumber - 1);
	return &dummyNonTransparentBlock;
}

Block* Block::getRightBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (x + 1 < CHUNKWIDTH)
		return world.getBlockAt(x + 1, y, z, chunkNumber);
	else if (chunkNumber % LOADEDCHUNKWIDTH != LOADEDCHUNKWIDTH - 1)
		return world.getBlockAt(0, y, z, chunkNumber + 1);
	return &dummyNonTransparentBlock;
}


Block* Block::getTopBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (y + 1 < CHUNKHEIGHT)
		return world.getBlockAt(x, y + 1, z, chunkNumber);
	return &dummyTransparentBlock;
}


Block* Block::getBottomBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (y - 1 > 0)
		return world.getBlockAt(x, y - 1, z, chunkNumber);
	return &dummyNonTransparentBlock;
}

Block* Block::getFrontBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (z - 1 >= 0)
		return world.getBlockAt(x, y, z - 1, chunkNumber);
	else if (chunkNumber - LOADEDCHUNKWIDTH >= 0)
		return world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkNumber - LOADEDCHUNKWIDTH);
	return &dummyNonTransparentBlock;
}

Block* Block::getBackBlock()
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (z + 1 < CHUNKWIDTH)
		return world.getBlockAt(x, y, z + 1, chunkNumber);
	else if (chunkNumber + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH)
		return world.getBlockAt(x, y, 0, chunkNumber + LOADEDCHUNKWIDTH);
	return &dummyNonTransparentBlock;
}



void Block::addGemometry(std::vector<float>& verts, std::vector<unsigned int>& triangles)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	switch (blockType)
	{
	case AIR:
		return;
	case DIRT: {
		Block* leftBlock = getLeftBlock(x, z);
		Block* rightBlock = getRightBlock(x, z);
		Block* topBlock = getTopBlock(x, z);
		Block* bottomBlock = getBottomBlock(x, z);
		Block* frontBlock = getFrontBlock(x, z);
		Block* backBlock = getBackBlock(x, z);
		if (leftBlock->blockType < 0)
			addLeftFace(verts, triangles, x, y, z, 2, 0, leftBlock);
		if (rightBlock->blockType < 0)
			addRightFace(verts, triangles, x, y, z, 2, 0, rightBlock);
		if (topBlock->blockType < 0)
			addTopFace(verts, triangles, x, y, z, 2, 0, topBlock);
		if (bottomBlock->blockType < 0)
			addBottomFace(verts, triangles, x, y, z, 2, 0, bottomBlock);
		if (frontBlock->blockType < 0)
			addFrontFace(verts, triangles, x, y, z, 2, 0, frontBlock);
		if (backBlock->blockType < 0)
			addBackFace(verts, triangles, x, y, z, 2, 0, backBlock);
		return;
	}
	default:
		break;
	}

}
