#ifndef WORLD_HPP
#define WORLD_HPP

#include <thread>
#include <mutex>
#include "world/Chunk.hpp"

#define CHUNK_HEIGHT 1
#define CHUNK_RADIUS 4

struct Vec3Comparator 
{
	bool operator()(const glm::ivec3& lhs, const glm::ivec3& rhs) const 
	{
		if (lhs.x < rhs.x) return true;
		if (lhs.x > rhs.x) return false;
		if (lhs.y < rhs.y) return true;
		if (lhs.y > rhs.y) return false;

		return lhs.z < rhs.z;
	}
};

namespace World
{
	extern std::map<glm::ivec3, Chunk*, Vec3Comparator> loadedChunks;
	extern std::mutex worldMutex;

	void LoadChunk(const glm::ivec3& position)
	{
		if (loadedChunks.find(position) != loadedChunks.end())
			return;

		Chunk* newChunk = new Chunk();
		newChunk->GenerateChunk(position * 16);

		loadedChunks[position] = newChunk;
	}

	void UnloadChunk(const glm::ivec3& position)
	{
		auto it = loadedChunks.find(position);

		if (it != loadedChunks.end())
		{
			delete it->second;
			loadedChunks.erase(it);
		}
	}

	void UpdateWorld(const glm::vec3& playerPosition)
	{
		glm::ivec3 currentChunk = glm::ivec3(
			floor(playerPosition.x / 16.0f),
			floor(playerPosition.y / 16.0f),
			floor(playerPosition.z / 16.0f)
		);

		for (int x = -CHUNK_RADIUS; x <= CHUNK_RADIUS; ++x)
		{
			for (int y = -CHUNK_HEIGHT; y <= CHUNK_HEIGHT; ++y)
			{
				for (int z = -CHUNK_RADIUS; z <= CHUNK_RADIUS; ++z)
					LoadChunk(currentChunk + glm::ivec3(x, y, z));
			}
		}

		auto it = loadedChunks.begin();

		while (it != loadedChunks.end())
		{
			glm::ivec3 diff = it->first - currentChunk;

			if (abs(diff.x) > CHUNK_RADIUS || abs(diff.y) > CHUNK_HEIGHT || abs(diff.z) > CHUNK_RADIUS)
			{
				delete it->second;
				it = loadedChunks.erase(it);
			}
			else
				++it;
		}
	}

	void CleanUp()
	{
		for (auto& pair : loadedChunks)
			delete pair.second;
		
		loadedChunks.clear();
	}
}

std::map<glm::ivec3, Chunk*, Vec3Comparator> World::loadedChunks;

#endif // !WORLD_HPP