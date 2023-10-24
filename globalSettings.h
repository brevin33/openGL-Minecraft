#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "block.h"
#include "shader.h"
#include "texture.h"
#define CHUNKWIDTH 16 // max 16 as block stores xz as a nibble/uint4_t
#define CHUNKHEIGHT 255 // max 255 as block stores y as uint8_t
#define LOADEDCHUNKWIDTH 3
#define TEXTUREATLASWIDTH 256.0
#define TEXTUREATLASHEIGHT 256.0
#define SPRITEWIDTH 16
class World;
class Chunk;
class Block;
extern World world;
extern std::vector<Shader> shaders;
extern Texture blockTexture;
extern Block dummyTransparentBlock;
extern Block dummyNonTransparentBlock;

enum ShaderNames
{
	block,
};
#define AIR -1
#define DIRT 0
#define GRASS 1
#endif