#include "Fov.hpp"
#include "Map.hpp"

#include <algorithm> // min, max

Fov::Slope::Slope(int y, int x)
	: x(x)
	, y(y)
{
}

bool Fov::Slope::greater(int y, int x) const
{
	return this->y * x > this->x * y;
}

bool Fov::Slope::greaterOrEqual(int y, int x) const
{
	return this->y * x >= this->x * y;
}

bool Fov::Slope::less(int y, int x) const
{
	return this->y * x < this->x * y;
}

bool Fov::Slope::lessOrEqual(int y, int x) const
{
	return this->y * x <= this->x * y;
}

void Fov::setAlgorithm(Algorithm algorithm)
{
	this->algorithm = algorithm;
}

Fov::Algorithm Fov::getAlgorithm() const
{
	return algorithm;
}

void Fov::setMap(Map& map)
{
	this->map = &map;
}

void Fov::compute(const sf::Vector2i& position, int range)
{
	for (int y = 0; y < map->height; ++y)
		for (int x = 0; x < map->width; ++x)
			map->at(x, y).visible = false;

	map->at(position).visible = true;
	map->at(position).explored = true;

	for (int octant = 0; octant < 8; ++octant)
	{
		switch (algorithm)
		{
		case Adam:
			compute(position, octant, range + 1, 1, Slope(1, 1), Slope(0, 1));
			break;

		case Bob:
			refreshOctant(position, octant, range + 1);
			break;
		}
	}
}

void Fov::compute(const sf::Vector2i& origin, int octant, int range, int x, Slope top, Slope bottom)
{
	for (; x <= range; ++x)
	{
		int topY = x;

		if (top.x != 1)
		{
			topY = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);

			if (blocksLight(x, topY, octant, origin))
			{
				if (top.greaterOrEqual(topY * 2 + 1, x * 2) && !blocksLight(x, topY + 1, octant, origin))
					topY += 1;
			}

			else
			{
				int ax = x * 2;

				if (blocksLight(x + 1, topY + 1, octant, origin))
					ax += 1;

				if (top.greater(topY * 2 + 1, ax))
					topY += 1;
			}
		}

		int bottomY = 0;

		if (bottom.y != 0)
		{
			bottomY = ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

			if (bottom.greaterOrEqual(bottomY * 2 + 1, x * 2) && blocksLight(x, bottomY, octant, origin) && !blocksLight(x, bottomY + 1, octant, origin))
				bottomY += 1;
		}

		int wasOpaque = -1;

		for (int y = topY; y >= bottomY; --y)
		{
			if (range >= 0 && x * x + y * y >= range * range) // getDistance(x, y) >= range
				continue;

			bool isOpaque = blocksLight(x, y, octant, origin);
			bool isVisible = isOpaque || ((y != topY || top.greater(y * 4 - 1, x * 4 + 1)) && (y != bottomY || bottom.less(y * 4 + 1, x * 4 - 1)));

			if (isVisible)
				setVisible(x, y, octant, origin);

			if (x == range)
				continue;

			if (isOpaque)
			{
				if (wasOpaque == 0)
				{
					int nx = x * 2;
					int ny = y * 2 + 1;

					if (algorithm == 2 && blocksLight(x, y + 1, octant, origin))
						nx -= 1;

					if (top.greater(ny, nx))
					{
						if (y == bottomY)
						{
							bottom = Slope(ny, nx);
							break;
						}

						else
							compute(origin, octant, range, x + 1, top, Slope(ny, nx));
					}

					else 
					{
						if (y == bottomY)
							return;
					}
				}

				wasOpaque = 1;
			}

			else
			{
				if (wasOpaque > 0)
				{
					int nx = x * 2;
					int ny = y * 2 + 1;

					if (algorithm == 2 && blocksLight(x + 1, y + 1, octant, origin))
						nx += 1;

					if (bottom.greaterOrEqual(ny, nx))
						return;

					top = Slope(ny, nx);
				}

				wasOpaque = 0;
			}
		}

		if (wasOpaque != 0)
			break;
	}
}

bool Fov::blocksLight(int x, int y, int octant, sf::Vector2i origin)
{
	switch (octant)
	{
	case 0: origin.x += x; origin.y -= y; break;
	case 1: origin.x += y; origin.y -= x; break;
	case 2: origin.x -= y; origin.y -= x; break;
	case 3: origin.x -= x; origin.y -= y; break;
	case 4: origin.x -= x; origin.y += y; break;
	case 5: origin.x -= y; origin.y += x; break;
	case 6: origin.x += y; origin.y += x; break;
	case 7: origin.x += x; origin.y += y; break;
	}

	return !map->isInBounds(origin) || !map->at(origin).transparent;
}

void Fov::setVisible(int x, int y, int octant, sf::Vector2i origin)
{
	switch (octant)
	{
	case 0: origin.x += x; origin.y -= y; break;
	case 1: origin.x += y; origin.y -= x; break;
	case 2: origin.x -= y; origin.y -= x; break;
	case 3: origin.x -= x; origin.y -= y; break;
	case 4: origin.x -= x; origin.y += y; break;
	case 5: origin.x -= y; origin.y += x; break;
	case 6: origin.x += y; origin.y += x; break;
	case 7: origin.x += x; origin.y += y; break;
	}

	if (map->isInBounds(origin))
	{
		map->at(origin).visible = true;
		map->at(origin).explored = true;
	}
}

bool Fov::Shadow::contains(const Shadow& projection) const
{
	return (start <= projection.start) && (end >= projection.end);
}

void Fov::refreshOctant(const sf::Vector2i& start, int octant, int range)
{
	sf::Vector2i rowInc;
	sf::Vector2i colInc;

	switch (octant)
	{
	case 0: rowInc.x =  0; rowInc.y = -1; colInc.x =  1, colInc.y =  0; break;
	case 1: rowInc.x =  1; rowInc.y =  0; colInc.x =  0, colInc.y = -1; break;
	case 2: rowInc.x =  1; rowInc.y =  0; colInc.x =  0, colInc.y =  1; break;
	case 3: rowInc.x =  0; rowInc.y =  1; colInc.x =  1, colInc.y =  0; break;
	case 4: rowInc.x =  0; rowInc.y =  1; colInc.x = -1, colInc.y =  0; break;
	case 5: rowInc.x = -1; rowInc.y =  0; colInc.x =  0, colInc.y =  1; break;
	case 6: rowInc.x = -1; rowInc.y =  0; colInc.x =  0, colInc.y = -1; break;
	case 7: rowInc.x =  0; rowInc.y = -1; colInc.x = -1, colInc.y =  0; break;
	}

	shadows.clear();
	bool fullShadow = false;

	for (int row = 1; row < range; ++row)
	{
		sf::Vector2i pos = start + (rowInc * row);

		if (!map->isInBounds(pos))
			break;

		for (int col = 0; col <= row; ++col)
		{
			sf::Vector2i vec = pos - start;

			// circular fov
			if (vec.x * vec.x + vec.y * vec.y >= range * range)
				continue;

			if (fullShadow)
				map->at(pos).visible = false;

			else
			{
				Shadow projection = getProjection(col, row);
				map->at(pos).visible = !isInShadow(projection);

				if (map->at(pos).visible)
					map->at(pos).explored = true;

				if (map->at(pos).visible && !map->at(pos).transparent) // blocksLight
					fullShadow = addShadow(projection);
			}

			pos += colInc;

			if (!map->isInBounds(pos))
				break;
		}
	}
}

Fov::Shadow Fov::getProjection(int col, int row)
{
	float topLeft = static_cast<float>(col) / (row + 2);
	float bottomRight = static_cast<float>(col + 1) / (row + 1);

	return { topLeft, bottomRight };
}

bool Fov::isInShadow(const Shadow& projection) const
{
	for (const auto& shadow : shadows)
	{
		if (shadow.contains(projection))
			return true;
	}

	return false;
}

bool Fov::addShadow(const Shadow& shadow)
{
	std::size_t index = 0;

	for (index = 0; index < shadows.size(); ++index)
	{
		if (shadows[index].start > shadow.start) // NOTE: > or >=
			break;
	}

	bool overlapsPrev = ((index > 0) && (shadows[index - 1].end > shadow.start));
	bool overlapsNext = ((index < shadows.size()) && (shadows[index].start < shadow.end));

	if (overlapsNext)
	{
		if (overlapsPrev)
		{
			shadows[index - 1].end = std::max(shadows[index - 1].end, shadows[index].end);
			shadows.erase(shadows.begin() + index);
		}

		else
			shadows[index].start = std::min(shadows[index].start, shadow.start);
	}

	else
	{
		if (overlapsPrev)
			shadows[index - 1].end = std::max(shadows[index - 1].end, shadow.end);

		else
			shadows.emplace(shadows.begin() + index, shadow);
	}

	return (shadows.size() == 1) && (shadows[0].start == 0) && (shadows[0].end == 1);
}