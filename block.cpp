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
		x, y, z,
		x + 1, y, z,
		x + 1, y - 1, z,
		x, y - 1, z,
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
	left = nullptr;
	up = nullptr;
	right = nullptr;
	down = nullptr;
	front = nullptr;
	back = nullptr;
}

Block::Block(int x, int y, int z,int blockType) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->blockType = blockType;
	left = nullptr;
	up = nullptr;
	right = nullptr;
	down = nullptr;
	front = nullptr;
	back = nullptr;
}

Block::~Block()
{
}


void Block::addGemometry(std::vector<float>& verts, std::vector<int>& triangles, std::vector<float>& uvs)
{
	if (up != nullptr)
		if(up->blockType < 0)
			addTopFace(verts,triangles,uvs,x,y,z);
	if (down != nullptr)
		if (down->blockType < 0)
			addBottomFace(verts, triangles, uvs, x, y, z);
	if (left != nullptr)
		if (left->blockType < 0)
			addLeftFace(verts, triangles, uvs, x, y, z);
	if (right != nullptr)
		if (right->blockType < 0)
			addRightFace(verts, triangles, uvs, x, y, z);
	if (front != nullptr)
		if (front->blockType < 0)
			addFrontFace(verts, triangles, uvs, x, y, z);
	if (back != nullptr)
		if (back->blockType < 0)
			addBackFace(verts, triangles, uvs, x, y, z);
}
