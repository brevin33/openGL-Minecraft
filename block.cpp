#include "block.h"
#include "world.h"
#include "globalSettings.h"
#include <vector>
inline void addTopFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x, y, z,
		x, y, z + 1,
		x + 1, y, z,
		x + 1, y, z + 1,
	};
	int topFaceTriangles[] = {
		size, size + 1, size + 3,
		size, size + 3, size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

inline void addBottomFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x, y-1, z,
		x, y-1, z + 1,
		x + 1, y-1, z,
		x + 1, y-1, z + 1,
	};
	int topFaceTriangles[] = {
		size, size + 3, size + 1,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

inline void addLeftFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x, y, z,
		x, y, z + 1,
		x, y - 1, z + 1,
		x, y - 1, z,
	};
	int topFaceTriangles[] = {
		size, size + 3, size + 2,
		size, size + 2,size + 1,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

inline void addRightFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x + 1, y, z,
		x + 1, y, z + 1,
		x + 1, y - 1, z + 1,
		x + 1, y - 1, z,
	};
	int topFaceTriangles[] = {
		size, size + 2, size + 3,
		size, size + 1,size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

inline void addFrontFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x, y, z - 1,
		x + 1, y, z - 1,
		x + 1, y - 1, z - 1,
		x, y - 1, z - 1,
	};
	int topFaceTriangles[] = {
		size, size + 2, size + 1,
		size, size + 3,size + 2,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

inline void addBackFace(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs, int x, int y, int z) {
	int size = triangles.size();
	float topFaceVerts[] = {
		x, y, z,
		x + 1, y, z,
		x + 1, y - 1, z,
		x, y - 1, z,
	};
	int topFaceTriangles[] = {
		size, size + 1, size + 2,
		size, size + 2,size + 3,
	};
	for (int i = 0; i < 12; i++)
	{
		verts.push_back(topFaceVerts[i]);
	}
	for (int i = 0; i < 6; i++)
	{
		triangles.push_back(topFaceTriangles[i]);
	}
}

Block::Block()
{
}

Block::Block(uint8_t x, uint8_t y, uint8_t z, int8_t blockType, uint8_t chunkNumber) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->blockType = blockType;
	this->chunkNumber = chunkNumber;
}

Block::~Block()
{
}


int8_t Block::getLeftBlockType()
{
	if (x - 1 > 0)
		return world.getBlockAt(x-1,y,z,chunkNumber)->blockType;
	else if(chunkNumber % LOADEDCHUNKWIDTH != 0)
		return world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkNumber - 1)->blockType;
	return -1;
}

int8_t Block::getRightBlockType()
{
	if (x + 1 < CHUNKWIDTH)
		return world.getBlockAt(x + 1, y, z, chunkNumber)->blockType;
	else if (chunkNumber % LOADEDCHUNKWIDTH != LOADEDCHUNKWIDTH - 1)
		return world.getBlockAt(0, y, z, chunkNumber + 1)->blockType;
	return -1;
}


int8_t Block::getTopBlockType()
{
	if (y + 1 < CHUNKHEIGHT)
		return world.getBlockAt(x, y + 1, z, chunkNumber)->blockType;
	return 1;
}


int8_t Block::getBottomBlockType()
{
	if (y - 1 > 0)
		return world.getBlockAt(x, y - 1, z, chunkNumber)->blockType;
	return -1;
}

int8_t Block::getFrontBlockType()
{
	if (z - 1 > 0)
		return world.getBlockAt(x, y, z - 1, chunkNumber)->blockType;
	else if (chunkNumber - LOADEDCHUNKWIDTH >= 0)
		return world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkNumber - LOADEDCHUNKWIDTH)->blockType;
	return -1;
}

int8_t Block::getBackBlockType()
{
	if (z - 1 > 0)
		return world.getBlockAt(x, y, z - 1, chunkNumber)->blockType;
	else if (chunkNumber + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH)
		return world.getBlockAt(x, y, 0, chunkNumber + LOADEDCHUNKWIDTH)->blockType;
	return -1;
}



void Block::addGemometry(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs)
{
	if (getLeftBlockType() < 0)
		addTopFace(verts,triangles,uvs,x,y,z);
	if (getRightBlockType() < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
	if (getTopBlockType() < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
	if (getBottomBlockType() < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
	if (getFrontBlockType() < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
	if (getBackBlockType() < 0)
		addTopFace(verts, triangles, uvs, x, y, z);
}
