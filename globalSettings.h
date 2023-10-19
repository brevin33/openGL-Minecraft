#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "block.h"
#define CHUNKWIDTH 16 // max 16 as block stores xz as a nibble/uint4_t
#define CHUNKHEIGHT 255 // max 255 as block stores y as uint8_t
#define LOADEDCHUNKWIDTH 16
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