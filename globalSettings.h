#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "block.h"
#include "shader.h"
#define CHUNKWIDTH 16 // max 16 as block stores xz as a nibble/uint4_t
#define CHUNKHEIGHT 255 // max 255 as block stores y as uint8_t
#define LOADEDCHUNKWIDTH 50
class World;
class Chunk;
class Block;
extern World world;
extern std::vector<Shader> shaders;
enum ShaderNames
{
	block,
};
enum BlockTypes
{
	AIR = -1,
	DIRT = 0,
	GRASS = 1,
};
#endif