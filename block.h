#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
class Block
{
public:
	Block();
	Block(int x, int y, int z,int blockType);
	~Block();
	void addGemometry(std::vector<float> &verts, std::vector<int>& triangles, std::vector<float>& uvs);
	Block* left, * right, * up, * down, * front, * back;
private:
	int blockType;
	int x, y, z;
};


#endif