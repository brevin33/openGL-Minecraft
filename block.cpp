#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include "nibble.h"
#include <vector>
inline void addTopFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x, y, z,
		x, y, z + 1,
		x + 1, y, z,
		x + 1, y, z + 1,
	};
	int faceTriangles[] = {
		size, size + 1, size + 3,
		size, size + 3, size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addBottomFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x, y-1, z,
		x, y-1, z + 1,
		x + 1, y-1, z,
		x + 1, y-1, z + 1,
	};
	int faceTriangles[] = {
		size, size + 3, size + 1,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addLeftFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x, y, z,
		x, y, z + 1,
		x, y - 1, z + 1,
		x, y - 1, z,
	};
	int faceTriangles[] = {
		size, size + 3, size + 2,
		size, size + 2,size + 1,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addRightFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x + 1, y, z,
		x + 1, y, z + 1,
		x + 1, y - 1, z + 1,
		x + 1, y - 1, z,
	};
	int faceTriangles[] = {
		size, size + 2, size + 3,
		size, size + 1,size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addFrontFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x, y, z - 1,
		x + 1, y, z - 1,
		x + 1, y - 1, z - 1,
		x, y - 1, z - 1,
	};
	int faceTriangles[] = {
		size, size + 2, size + 1,
		size, size + 3,size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(faceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(faceTriangles[i]);
	}
}

inline void addBackFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, uint8_t x, uint8_t y, uint8_t z) {
	int size = triangles.size();
	float faceVerts[] = {
		x, y, z,
		x + 1, y, z,
		x + 1, y - 1, z,
		x, y - 1, z,
	};
	int faceTriangles[] = {
		size, size + 1, size + 2,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 12; i++)
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



void Block::addGemometry(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs)
{
	if (blockType == AIR) return;
	uint8_t x = LO_NIBBLE(xz);
	uint8_t z = HI_NIBBLE(xz);
	if (getLeftBlockType(x,z) < 0)
		addLeftFace(verts,triangles,uvs,x,y,z);
	if (getRightBlockType(x,z) < 0)
		addRightFace(verts, triangles, uvs, x, y, z);
	if (getTopBlockType(x,z) < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
	if (getBottomBlockType(x,z) < 0)
		addBottomFace(verts, triangles, uvs, x, y, z);
	if (getFrontBlockType(x,z) < 0)
		addFrontFace(verts, triangles, uvs, x, y, z);
	if (getBackBlockType(x,z) < 0)
		addBackFace(verts, triangles, uvs, x, y, z);
}
