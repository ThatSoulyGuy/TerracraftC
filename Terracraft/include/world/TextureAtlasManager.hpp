#ifndef TEXTURE_ATLAS_MANAGER_HPP
#define TEXTURE_ATLAS_MANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include "rendering/TextureManager.hpp"

#pragma warning(disable : 26498)

namespace TextureAtlasManager
{
	extern Texture atlas;
	int squareSize = 16;
    int atlasSize = 256;
	float PADDING_RATIO = 0.1f / (float)atlasSize;

	void Initalize()
	{
		atlas = TextureManager::GetTexture("terrain_atlas");
	}

	std::vector<glm::vec2> GetTextureCoordinates(const glm::ivec2& position, float rotation = 0)
	{
        if (atlasSize == 0) return {};

        float x0 = (float)position.x * squareSize / atlasSize;
        float y0 = (float)position.y * squareSize / atlasSize;

        float x1 = x0 + (float)squareSize / atlasSize;
        float y1 = y0;

        float x2 = x0 + (float)squareSize / atlasSize;
        float y2 = y0 + (float)squareSize / atlasSize;

        float x3 = x0;
        float y3 = y0 + (float)squareSize / atlasSize;

        float centerX = (x0 + x2) / 2.0f;
        float centerY = (y0 + y2) / 2.0f;

        x0 -= centerX;
        y0 -= centerY;
        x1 -= centerX;
        y1 -= centerY;
        x2 -= centerX;
        y2 -= centerY;
        x3 -= centerX;
        y3 -= centerY;

        float rotationAngleRadians = glm::radians(rotation);

        float cosTheta = cos(rotationAngleRadians);
        float sinTheta = sin(rotationAngleRadians);

        float x0Rotated = x0 * cosTheta - y0 * sinTheta;
        float y0Rotated = x0 * sinTheta + y0 * cosTheta;

        float x1Rotated = x1 * cosTheta - y1 * sinTheta;
        float y1Rotated = x1 * sinTheta + y1 * cosTheta;

        float x2Rotated = x2 * cosTheta - y2 * sinTheta;
        float y2Rotated = x2 * sinTheta + y2 * cosTheta;

        float x3Rotated = x3 * cosTheta - y3 * sinTheta;
        float y3Rotated = x3 * sinTheta + y3 * cosTheta;

        x0Rotated += centerX;
        y0Rotated += centerY;
        x1Rotated += centerX;
        y1Rotated += centerY;
        x2Rotated += centerX;
        y2Rotated += centerY;
        x3Rotated += centerX;
        y3Rotated += centerY;

        return
        {
            {x0Rotated, y0Rotated},
            {x1Rotated, y1Rotated},
            {x2Rotated, y2Rotated},
            {x3Rotated, y3Rotated}
        };
	}
}

Texture TextureAtlasManager::atlas;

#endif // !TEXTURE_ATLAS_MANAGER_HPP