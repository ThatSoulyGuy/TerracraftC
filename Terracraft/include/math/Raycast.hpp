#ifndef RAYCAST_HPP
#define RAYCAST_HPP

#include "gameplay/Player.hpp"
#include "world/Chunk.hpp"

struct Raycast
{
	glm::vec3 origin;
	glm::vec3 direction;

    glm::ivec3 CheckBlock(Chunk& chunk, float maxDistance, float stepSize) 
    {
        float distance = 0.0f;

        while (distance < maxDistance)
        {
            glm::vec3 point = origin + direction * distance;
            glm::ivec3 blockCoords = glm::ivec3(glm::floor(point));

            if (blockCoords.x < 0 || blockCoords.x >= CHUNK_SIZE || blockCoords.y < 0 || blockCoords.y >= CHUNK_SIZE || blockCoords.z < 0 || blockCoords.z >= CHUNK_SIZE)
            {
                distance += stepSize;
                continue;
            }

            int block = chunk.blocks[blockCoords.x][blockCoords.y][blockCoords.z];

            if (block != BlockType::blocks["block_air"])
                return blockCoords;

            distance += stepSize;
        }

        return { -1, -1, -1 };
    }

    static Raycast Register(const Camera& camera) 
    {
        Raycast out = {};

        glm::vec3 direction = glm::normalize(glm::vec3(
            cos(glm::radians(camera.data.yaw)) * cos(glm::radians(camera.data.pitch)),
            sin(glm::radians(camera.data.pitch)),
            sin(glm::radians(camera.data.yaw)) * cos(glm::radians(camera.data.pitch))
        ));

        out.origin = camera.data.transform.position;
        out.direction = direction;

        return out;
    }
};

#endif // !RAYCAST_HPP