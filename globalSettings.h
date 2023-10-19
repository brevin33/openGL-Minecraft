#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "block.h"
#define CHUNKWIDTH 256
#define CHUNKHEIGHT 256
#define LOADEDCHUNKWIDTH 9
class World;
class Chunk;
class Block;
extern World world;
enum BlockTypes
{
	AIR = -1,
	DIRT = 0,
	GRASS = 1,
};
#endif