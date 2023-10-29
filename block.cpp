#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include "nibble.h"
#include <vector>

Block dummyTransparentBlock(0,0,0,-1);
Block dummyNonTransparentBlock(0, 0, 0, 1);

inline int isTransparent(ChunkBlock block) {
	return block.block.blockType >= 0 ? 1 : 0;
}

inline float* ambientOcclusionTopAndBottomFace(ChunkBlock block) {
	float occlusionValues[4];
	ChunkBlock frontBlock = block.block.getFrontBlock(block.chunkNumber);
	ChunkBlock backBlock = block.block.getBackBlock(block.chunkNumber);
	ChunkBlock leftBlock = block.block.getLeftBlock(block.chunkNumber);
	ChunkBlock rightBlock = block.block.getRightBlock(block.chunkNumber);
	ChunkBlock frontLeftCorrner = frontBlock.block.getLeftBlock(frontBlock.chunkNumber);
	ChunkBlock frontRightCorrner = frontBlock.block.getRightBlock(frontBlock.chunkNumber);
	ChunkBlock backLeftCorrner = backBlock.block.getLeftBlock(backBlock.chunkNumber);
	ChunkBlock backRightCorrner = backBlock.block.getRightBlock(backBlock.chunkNumber);
	occlusionValues[0] = isTransparent(backBlock) + isTransparent(leftBlock) + isTransparent(backLeftCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(frontBlock) + isTransparent(leftBlock) + isTransparent(frontLeftCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(backBlock) + isTransparent(rightBlock) + isTransparent(backRightCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(frontBlock) + isTransparent(rightBlock) + isTransparent(frontRightCorrner) * 0.5f;
	return occlusionValues;
}

inline void addTopFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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


inline void addBottomFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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

inline float* ambientOcclusionLeftAndRightFace(ChunkBlock block) {
	float occlusionValues[4];
	ChunkBlock frontBlock = block.block.getFrontBlock(block.chunkNumber);
	ChunkBlock backBlock = block.block.getBackBlock(block.chunkNumber);
	ChunkBlock topBlock = block.block.getTopBlock(block.chunkNumber);
	ChunkBlock bottomBlock = block.block.getBottomBlock(block.chunkNumber);
	ChunkBlock frontTopCorrner = frontBlock.block.getTopBlock(frontBlock.chunkNumber);
	ChunkBlock frontBottomCorrner = frontBlock.block.getBottomBlock(frontBlock.chunkNumber);
	ChunkBlock backTopCorrner = backBlock.block.getTopBlock(backBlock.chunkNumber);
	ChunkBlock backBottomCorrner = backBlock.block.getBottomBlock(backBlock.chunkNumber);
	occlusionValues[0] = isTransparent(backBlock) + isTransparent(topBlock) + isTransparent(backTopCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(frontBlock) + isTransparent(topBlock) + isTransparent(frontTopCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(frontBlock) + isTransparent(bottomBlock) + isTransparent(frontBottomCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(backBlock) + isTransparent(bottomBlock) + isTransparent(backBottomCorrner) * 0.5f;
	return occlusionValues;
}


inline void addLeftFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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


inline void addRightFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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

inline float* ambientOcclusionFrontAndBackFace(ChunkBlock block) {
	float occlusionValues[4];
	ChunkBlock leftBlock = block.block.getLeftBlock(block.chunkNumber);
	ChunkBlock rightBlock = block.block.getRightBlock(block.chunkNumber);
	ChunkBlock topBlock = block.block.getTopBlock(block.chunkNumber);
	ChunkBlock bottomBlock = block.block.getBottomBlock(block.chunkNumber);
	ChunkBlock leftTopCorrner = leftBlock.block.getTopBlock(leftBlock.chunkNumber);
	ChunkBlock leftBottomCorrner = leftBlock.block.getBottomBlock(leftBlock.chunkNumber);
	ChunkBlock rightTopCorrner = rightBlock.block.getTopBlock(rightBlock.chunkNumber);
	ChunkBlock rightBottomCorrner = rightBlock.block.getBottomBlock(rightBlock.chunkNumber);
	occlusionValues[0] = isTransparent(leftBlock) + isTransparent(topBlock) + isTransparent(leftTopCorrner) * 0.5f;
	occlusionValues[1] = isTransparent(rightBlock) + isTransparent(topBlock) + isTransparent(rightTopCorrner) * 0.5f;
	occlusionValues[2] = isTransparent(rightBlock) + isTransparent(bottomBlock) + isTransparent(rightBottomCorrner) * 0.5f;
	occlusionValues[3] = isTransparent(leftBlock) + isTransparent(bottomBlock) + isTransparent(leftBottomCorrner) * 0.5f;
	return occlusionValues;
}

inline void addFrontFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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

inline void addBackFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint8_t x, uint8_t y, uint8_t z, uint8_t suvx, uint8_t suvy, ChunkBlock block) {
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

Block::Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType) {
	xz = 0;
	xz = setLowerNibble(xz, x);
	xz = setUpperNibble(xz, z);
	this->y = y;
	this->blockType = blockType;
}

Block::~Block()
{
}


ChunkBlock Block::getLeftBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (x - 1 >= 0) {
		c.block = world.getBlockAt(x - 1, y, z, chunkNumber);
		c.chunkNumber = chunkNumber;
		return c;
	}
	else if (chunkNumber % LOADEDCHUNKWIDTH != 0) {
		c.block = world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkNumber - 1);
		c.chunkNumber = chunkNumber - 1;
		return c;
	}
	c.block = dummyNonTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}

ChunkBlock Block::getRightBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (x + 1 < CHUNKWIDTH) {
		 c.block = world.getBlockAt(x + 1, y, z, chunkNumber);
		 c.chunkNumber = chunkNumber;
		 return c;
	}
	else if (chunkNumber % LOADEDCHUNKWIDTH != LOADEDCHUNKWIDTH - 1) {
		c.block =  world.getBlockAt(0, y, z, chunkNumber + 1);
		c.chunkNumber = chunkNumber + 1;
		return c;
	}
	c.block = dummyNonTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}


ChunkBlock Block::getTopBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (y + 1 < CHUNKHEIGHT) {
		c.block = world.getBlockAt(x, y + 1, z, chunkNumber);
		c.chunkNumber = chunkNumber;
		return c;
	}
	c.block = dummyTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}


ChunkBlock Block::getBottomBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (y - 1 > 0) {
		c.block = world.getBlockAt(x, y - 1, z, chunkNumber);
		c.chunkNumber = chunkNumber;
		return c;
	}
	c.block = dummyNonTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}

ChunkBlock Block::getFrontBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (z - 1 >= 0) {
		c.block = world.getBlockAt(x, y, z - 1, chunkNumber);
		c.chunkNumber = chunkNumber;
		return c;
	}
	else if (chunkNumber - LOADEDCHUNKWIDTH >= 0) {
		c.block = world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkNumber - LOADEDCHUNKWIDTH);
		c.chunkNumber = chunkNumber - LOADEDCHUNKWIDTH;
		return c;
	}
	c.block = dummyNonTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}

ChunkBlock Block::getBackBlock(uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	ChunkBlock c;
	if (z + 1 < CHUNKWIDTH) {
		c.chunkNumber = chunkNumber;
		c.block = world.getBlockAt(x, y, z + 1, chunkNumber);
		return c;
	}
	else if (chunkNumber + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH) {
		c.block = world.getBlockAt(x, y, 0, chunkNumber + LOADEDCHUNKWIDTH);
		c.chunkNumber = chunkNumber + LOADEDCHUNKWIDTH;
		return c;
	}
	c.block = dummyNonTransparentBlock;
	c.chunkNumber = chunkNumber;
	return c;
}



void Block::addGemometry(std::vector<float>& verts, std::vector<unsigned int>& triangles, uint16_t chunkNumber)
{
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	switch (blockType)
	{
	case AIR:
		return;
	case DIRT: {
		ChunkBlock leftBlock = getLeftBlock(chunkNumber);
		ChunkBlock rightBlock = getRightBlock(chunkNumber);
		ChunkBlock topBlock = getTopBlock(chunkNumber);
		ChunkBlock bottomBlock = getBottomBlock(chunkNumber);
		ChunkBlock frontBlock = getFrontBlock(chunkNumber);
		ChunkBlock backBlock = getBackBlock(chunkNumber);
		if (leftBlock.block.blockType < 0)
			addLeftFace(verts, triangles, x, y, z, 14, 28, leftBlock);
		if (rightBlock.block.blockType < 0)
			addRightFace(verts, triangles, x, y, z, 14, 28, rightBlock);
		if (topBlock.block.blockType < 0)
			addTopFace(verts, triangles, x, y, z, 14, 28, topBlock);
		if (bottomBlock.block.blockType < 0)
			addBottomFace(verts, triangles, x, y, z, 14, 28, bottomBlock);
		if (frontBlock.block.blockType < 0)
			addFrontFace(verts, triangles, x, y, z, 14, 28, frontBlock);
		if (backBlock.block.blockType < 0)
			addBackFace(verts, triangles, x, y, z, 14, 28, backBlock);
		return;
	}
	default:
		break;
	}

}
