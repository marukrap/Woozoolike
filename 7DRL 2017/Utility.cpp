#include "Utility.hpp"

#include <random>
#include <cwctype> // towupper

namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());
}

int randomInt(int exclusiveMax)
{
	std::uniform_int_distribution<> dist(0, exclusiveMax - 1);

	return dist(mt);
}

int randomInt(int min, int inclusiveMax)
{
	std::uniform_int_distribution<> dist(min, inclusiveMax);

	return dist(mt);
}

int rollDice(int n, int s)
{
	int value = 0;

	for (int i = 0; i < n; ++i)
		value += randomInt(1, s);

	return value;
}

std::wstring capitalize(const std::wstring& string)
{
	std::wstring result = string;

	if (!result.empty())
		result[0] = std::towupper(result[0]);

	return result;
}

std::vector<sf::Vector2i> plotPath(sf::Vector2i p1, sf::Vector2i p2)
{
	std::vector<sf::Vector2i> path;

	int dx = std::abs(p2.x - p1.x);
	int dy = -std::abs(p2.y - p1.y);

	int sx = p1.x < p2.x ? 1 : -1;
	int sy = p1.y < p2.y ? 1 : -1;

	// error value e_xy
	int err = dx + dy;

	if (p1 == p2)
		path.emplace_back(p1);

	while (p1 != p2)
	{
		int e2 = 2 * err;

		if (e2 >= dy) // e_xy + e_x > 0
		{
			err += dy;
			p1.x += sx;
		}

		if (e2 <= dx) // e_xy + e_y > 0
		{
			err += dx;
			p1.y += sy;
		}

		path.emplace_back(p1);
	}

	return path;
}