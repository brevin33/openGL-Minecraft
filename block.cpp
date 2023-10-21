#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include "nibble.h"
#include <vector>
inline void addTopFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx+1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy+1) * 16) / 256.0;

	float faceVerts[] = {
		x, y, z, uvx, uvy,
		x, y, z + 1, uvx, uvy2,
		x + 1, y, z, uvx2, uvy,
		x + 1, y, z + 1, uvx2, uvy2,
	};
	int faceTriangles[] = {
		size, size + 1, size + 3,
		size, size + 3, size + 2,
	};
	for (int i = 0; i < 20; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addBottomFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx + 1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy + 1) * 16) / 256.0;
	float faceVerts[] = {
		x, y-1, z, uvx, uvy,
		x, y-1, z + 1, uvx, uvy2,
		x + 1, y-1, z, uvx2, uvy,
		x + 1, y-1, z + 1, uvx2, uvy + (1.0 / 16.0),
	};
	int faceTriangles[] = {
		size, size + 3, size + 1,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 20; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addLeftFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx + 1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy + 1) * 16) / 256.0;
	float faceVerts[] = {
		x, y, z, uvx, uvy,
		x, y, z + 1, uvx, uvy2,
		x, y - 1, z + 1, uvx2, uvy2,
		x, y - 1, z, uvx2, uvy,
	};
	int faceTriangles[] = {
		size, size + 3, size + 2,
		size, size + 2,size + 1,
	};
	for (int i = 0; i < 20; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addRightFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx + 1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy + 1) * 16) / 256.0;
	float faceVerts[] = {
		x + 1, y, z, uvx, uvy,
		x + 1, y, z + 1, uvx, uvy2,
		x + 1, y - 1, z + 1, uvx2, uvy2,
		x + 1, y - 1, z, uvx2, uvy,
	};
	int faceTriangles[] = {
		size, size + 2, size + 3,
		size, size + 1,size + 2,
	};
	for (int i = 0; i < 20; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addFrontFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx + 1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy + 1) * 16) / 256.0;
	float faceVerts[] = {
		x, y, z - 1, uvx, uvy,
		x + 1, y, z - 1, uvx, uvy2,
		x + 1, y - 1, z - 1, uvx2, uvy2,
		x, y - 1, z - 1, uvx2, uvy,
	};
	int faceTriangles[] = {
		size, size + 2, size + 1,
		size, size + 3,size + 2,
	};
	for (int i = 0; i < 20; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addBackFace(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z, int suvx, int suvy) {
	int size = verts.size()/5;
	float uvx = (suvx * 16) / 256.0;
	float uvx2 = ((suvx + 1) * 16) / 256.0;
	float uvy = (suvy * 16) / 256.0;
	float uvy2 = ((suvy + 1) * 16) / 256.0;
	float faceVerts[] = {
		x, y, z, uvx, uvy,
		x + 1, y, z, uvx, uvy2,
		x + 1, y - 1, z, uvx2, uvy2,
		x, y - 1, z, uvx2, uvy,
	};
	int faceTriangles[] = {
		size, size + 1, size + 2,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 20; i++)
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


int8_t Block::getLeftBlockType(uint8_t x, uint8_t z)
{
	if (x - 1 > 0)
		return world.getBlockAt(x-1,y,z,chunkNumber)->blockType;
	else if(chunkNumber % LOADEDCHUNKWIDTH != 0)
		return world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkNumber - 1)->blockType;
	return 1;
}

int8_t Block::getRightBlockType(uint8_t x, uint8_t z)
{
	if (x + 1 < CHUNKWIDTH)
		return world.getBlockAt(x + 1, y, z, chunkNumber)->blockType;
	else if (chunkNumber % LOADEDCHUNKWIDTH != LOADEDCHUNKWIDTH - 1)
		return world.getBlockAt(0, y, z, chunkNumber + 1)->blockType;
	return 1;
}


int8_t Block::getTopBlockType(uint8_t x, uint8_t z)
{
	if (y + 1 < CHUNKHEIGHT)
		return world.getBlockAt(x, y + 1, z, chunkNumber)->blockType;
	return -1;
}


int8_t Block::getBottomBlockType(uint8_t x, uint8_t z)
{
	if (y - 1 > 0)
		return world.getBlockAt(x, y - 1, z, chunkNumber)->blockType;
	return 1;
}

int8_t Block::getFrontBlockType(uint8_t x, uint8_t z)
{
	if (z - 1 > 0)
		return world.getBlockAt(x, y, z - 1, chunkNumber)->blockType;
	else if (chunkNumber - LOADEDCHUNKWIDTH >= 0)
		return world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkNumber - LOADEDCHUNKWIDTH)->blockType;
	return 1;
}

int8_t Block::getBackBlockType(uint8_t x, uint8_t z)
{
	if (z + 1 < CHUNKWIDTH)
		return world.getBlockAt(x, y, z + 1, chunkNumber)->blockType;
	else if (chunkNumber + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH)
		return world.getBlockAt(x, y, 0, chunkNumber + LOADEDCHUNKWIDTH)->blockType;
	return 1;
}



void Block::addGemometry(std::vector<float>& verts, std::vector<unsigned int>& triangles, std::vector<float>& uvs)
{
	if (blockType == AIR) return;
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (getLeftBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addLeftFace(verts, triangles, uvs, x, y, z,2,0);
			break;

		default:
			break;
		}
	if (getRightBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addRightFace(verts, triangles, uvs, x, y, z, 2, 0);
			break;

		default:
			break;
		}
	if (getTopBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addTopFace(verts, triangles, uvs, x, y, z, 2, 0);
			break;

		default:
			break;
		}
	if (getBottomBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addBottomFace(verts, triangles, uvs, x, y, z, 2, 0);
			break;

		default:
			break;
		}
	if (getFrontBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addFrontFace(verts, triangles, uvs, x, y, z, 2, 0);
			break;

		default:
			break;
		}
	if (getBackBlockType(x,z) < 0)
		switch (blockType)
		{
		case DIRT:
			addBackFace(verts, triangles, uvs, x, y, z, 2, 0);
			break;

		default:
			break;
		}
}
