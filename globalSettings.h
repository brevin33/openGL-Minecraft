#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "block.h"
#include "shader.h"
#include "texture.h"
#define CHUNKWIDTH 16 // max 16 as block stores xz as a nibble/uint4_t
#define CHUNKHEIGHT 128 // max 255 as block stores y as uint8_t
#define LOADEDCHUNKWIDTH 17
#define TEXTUREATLASWIDTH 1024.0
#define TEXTUREATLASHEIGHT 512.0
#define SPRITEWIDTH 16
#define BLOCKVAOCOUNT 24
#define AMBIENTOCCLUSIONSTRENGTH .33f
#define CHUNKBUFFERSIZE 12000
// blocks
#define LEAF -6
#define WATER -2
#define AIR -1
#define DIRT 0
#define GRASS 1
#define STONE 2
#define LOG 3
#define PLANK 4
#define SAND 5
class World;
class Chunk;
class Block;
inline glm::vec3 MoveTowards(glm::vec3 start, glm::vec3 dest, float max);
extern World world;
extern std::vector<Shader> shaders;
extern Texture blockTexture;
extern Block dummyTransparentBlock;
extern Block dummyNonTransparentBlock;
extern bool changeChunkLoaded;
class Block;

enum ShaderNames
{
	blockShader,
	screenSpace,
	skybox,
};

#endif