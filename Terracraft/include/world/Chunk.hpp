#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "rendering/Renderer.hpp"
#include "world/BlockType.hpp"
#include "world/TextureAtlasManager.hpp"

#define CHUNK_SIZE 16

struct Chunk
{
	RenderableObject object = {};

	std::vector<Vertex> vertices = {};
	std::vector<unsigned int> indices = {};
	int indiceIndex = 0;

	int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	void GenerateChunk()
	{
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					if (y == 15)
						blocks[x][y][z] = BlockType::blocks["block_grass"];
					else if (y < 15 && y > 11)
						blocks[x][y][z] = BlockType::blocks["block_dirt"];
					else if (y < 12)
						blocks[x][y][z] = BlockType::blocks["block_stone"];
					//blocks[x][y][z] = BlockType::blocks["block_grass"];
				}
			}
		}

		Rebuild();
	}

	void Rebuild()
	{
		vertices.clear();
		indices.clear();
		indiceIndex = 0;

		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					if (blocks[x][y][z] == BlockType::blocks["block_air"])
						continue;

					auto textureCoordinates = BlockType::GetCoordinates(blocks[x][y][z]);

					if (y + 1 >= CHUNK_SIZE || blocks[x][y + 1][z] == BlockType::blocks["block_air"])
						GenerateTopFace({ x, y, z }, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[0]));

					if (y - 1 < 0 || blocks[x][y - 1][z] == BlockType::blocks["block_air"])
						GenerateBottomFace({ x, y, z }, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[1]));

					if (z + 1 >= CHUNK_SIZE || blocks[x][y][z + 1] == BlockType::blocks["block_air"])
						GenerateFrontFace({ x, y, z + 1 }, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[2], 90));

					if (z - 1 < 0 || blocks[x][y][z - 1] == BlockType::blocks["block_air"])
						GenerateBackFace({ x, y, z - 1}, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[3], 90));

					if (x + 1 >= CHUNK_SIZE || blocks[x + 1][y][z] == BlockType::blocks["block_air"])
						GenerateRightFace({ x + 1, y, z }, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[4], 90));

					if (x - 1 < 0 || blocks[x - 1][y][z] == BlockType::blocks["block_air"])
						GenerateLeftFace({ x - 1, y, z }, TextureAtlasManager::GetTextureCoordinates(textureCoordinates[5], 90));
				}
			}
		}

		object = RenderableObject::Register("chunk", vertices, indices);
		object.RegisterTexture("default_texture", TextureAtlasManager::atlas);//TextureManager::GetTexture("test_texture")
		object.GenerateRawData();

		Renderer::RegisterRenderableObject(object);
	}

	void SetBlock(const glm::ivec3& position, const std::string& name)
	{
		if (abs(position.x) >= CHUNK_SIZE || abs(position.y) >= CHUNK_SIZE || abs(position.z) >= CHUNK_SIZE || name == "")
			return;

		blocks[position.x][position.y][position.z] = BlockType::blocks[name];

		Rebuild();
	}

	void GenerateTopFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(2 + indiceIndex);

		indiceIndex += 4;
	}

	void GenerateBottomFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(indiceIndex);

		indices.push_back(2 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indiceIndex += 4;
	}

	void GenerateFrontFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(indiceIndex);

		indices.push_back(2 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indiceIndex += 4;
	}

	void GenerateBackFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(2 + indiceIndex);

		indiceIndex += 4;
	}

	void GenerateRightFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 0.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(2 + indiceIndex);

		indiceIndex += 4;
	}

	void GenerateLeftFace(const glm::vec3& position, const std::vector<glm::vec2>& uvs)
	{
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[0].x, uvs[0].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 0.0f + position.z }, DEFAULT_COLOR, { uvs[1].x, uvs[1].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 1.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[2].x, uvs[2].y }));
		vertices.push_back(Vertex::Register({ 1.0f + position.x, 0.0f + position.y, 1.0f + position.z }, DEFAULT_COLOR, { uvs[3].x, uvs[3].y }));

		indices.push_back(3 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(indiceIndex);

		indices.push_back(2 + indiceIndex);
		indices.push_back(1 + indiceIndex);
		indices.push_back(3 + indiceIndex);

		indiceIndex += 4;
	}
};

#endif // !CHUNK_HPP