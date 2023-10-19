#include "chunk.h"
#include "block.h"
#include "globalSettings.h"
#include "world.h"

Block dummyTranparent(0, 0, 0, -1);
Block dummyNotTransparent(0, 0, 0, 1);

Chunk::Chunk(){

}

Chunk::Chunk(int worldZ, int worldX, int chunkIndex)
{
    this->wx = worldX;
    this->wz = worldZ;
    this->chunkIndex = chunkIndex;
	for (int x = 0; x < CHUNKWIDTH; x++)
	{
		for (int y = 0; y < CHUNKHEIGHT; y++)
		{
			for (int z = 0; z < CHUNKWIDTH; z++)
			{
                int blockI = z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x;
				blocks[blockI] = GenorateBlock(x, y, z);
			}
		}
	}
}

Chunk::~Chunk()
{
}

void Chunk::createMesh() {
	vertices.clear();
	triangles.clear();
	uvs.clear();
	for (int i = 0; i < CHUNKWIDTH * CHUNKHEIGHT; i++)
	{
        Block b = blocks[i];
		b.addGemometry(vertices,triangles,uvs);
	}
	// send to gpu
}

void Chunk::giveBlocksNeighbors()
{

    for (int x = 0; x < CHUNKWIDTH - 1; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].right = &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + (x+1)];
            }
        }
    }

    for (int x = CHUNKWIDTH - 1; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                if ((chunkIndex + 1) % LOADEDCHUNKWIDTH == 0)
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].right = nullptr;
                else {
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].right = world.getBlockAt(0, y, z, chunkIndex + 1);
                }
            }
        }
    }

    for (int x = 1; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].left = &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + (x - 1)];
            }
        }
    }

    for (int x = 0; x < 1; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                if (chunkIndex % LOADEDCHUNKWIDTH == 0)
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].left = nullptr;
                else {
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].left = world.getBlockAt(CHUNKWIDTH - 1, y, z, chunkIndex - 1);
                }
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT - 1; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].up = &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + (y + 1) * CHUNKWIDTH + x];
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = CHUNKHEIGHT - 1; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].up = &dummyTranparent;
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 1; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].down = &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + (y - 1) * CHUNKWIDTH + x];
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < 1; y++) {
            for (int z = 0; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].down = &dummyNotTransparent;
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < CHUNKWIDTH - 1; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].front = &blocks[(z + 1) * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x];
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = CHUNKWIDTH - 1; z < CHUNKWIDTH; z++) {
                if (chunkIndex + LOADEDCHUNKWIDTH < LOADEDCHUNKWIDTH * LOADEDCHUNKWIDTH)
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].back = nullptr;
                else {
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].front = world.getBlockAt(x, y, 0, chunkIndex + LOADEDCHUNKWIDTH);
                }
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 1; z < CHUNKWIDTH; z++) {
                blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].front = &blocks[(z - 1) * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x];
            }
        }
    }

    for (int x = 0; x < CHUNKWIDTH; x++) {
        for (int y = 0; y < CHUNKHEIGHT; y++) {
            for (int z = 0; z < 1; z++) {
                if (chunkIndex - LOADEDCHUNKWIDTH < 0)
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].back = nullptr;
                else {
                    blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x].back = world.getBlockAt(x, y, CHUNKWIDTH - 1, chunkIndex - LOADEDCHUNKWIDTH);
                }
            }
        }
    }
}

void Chunk::update(float dt)
{
}

Block* Chunk::getBlockAt(int x, int y, int z)
{
    return &blocks[z * CHUNKHEIGHT * CHUNKWIDTH + y * CHUNKWIDTH + x];
}

Block Chunk::GenorateBlock(int x, int y, int z) {
	return Block(x, y, z, 1);
}
