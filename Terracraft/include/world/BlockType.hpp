#ifndef BLOCK_TYPE_HPP
#define BLOCK_TYPE_HPP

#include <vector>
#include <map>
#include <glm/glm.hpp> 

namespace BlockType
{
	std::map<std::string, int> blocks;

	void Initalize()
	{
		blocks =
		{
			{"block_air", 0},
			{"block_grass", 1},
			{"block_dirt", 2},
			{"block_stone", 3},
		};
	}

	std::vector<glm::vec2> GetCoordinates(int block)
	{
		switch (block)
		{

		case 0:

			return
			{
				{6, 13},
				{6, 13},
				{6, 13},
				{6, 13},
				{6, 13},
				{6, 13}
			};

		case 1:

			return
			{
				{0, 15},
				{2, 15},
				{3, 15},
				{3, 15},
				{3, 15},
				{3, 15}
			};

		case 2:

			return
			{
				{2, 15},
				{2, 15},
				{2, 15},
				{2, 15},
				{2, 15},
				{2, 15}
			};

		case 3:

			return
			{
				{1, 15},
				{1, 15},
				{1, 15},
				{1, 15},
				{1, 15},
				{1, 15}
			};

		default:
			break;
		}
	}
}

#endif // !BLOCK_TYPE_HPP