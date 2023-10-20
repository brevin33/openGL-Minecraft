#ifndef TEXTURE_H
#define TEXTURE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
class Texture
{
public:
	Texture(std::string path, bool flip = false);

	void bind(int textureNum = 0);

private:
	unsigned int texture;
};

#endif