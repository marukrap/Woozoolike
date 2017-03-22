#include "DijkstraMap.hpp"
#include "Map.hpp"
#include "Direction.hpp"

void DijkstraMap::setMap(Map& map)
{
	this->map = &map;
	size.x = map.width;
	size.y = map.height;
	values.resize(size.x * size.y);
}

void DijkstraMap::set(int x, int y, int value)
{
	values[x + y * size.x] = value;
}

void DijkstraMap::set(const sf::Vector2i& position, int value)
{
	set(position.x, position.y, value);
}

int DijkstraMap::get(int x, int y) const
{
	return values[x + y * size.x];
}

int DijkstraMap::get(const sf::Vector2i& position) const
{
	return get(position.x, position.y);
}

sf::Vector2i DijkstraMap::getNextDir(const sf::Vector2i& position) const
{
	sf::Vector2i nextDir;
	int lowest = get(position);

	for (auto dir : Direction::All)
	{
		if (!map->isInBounds(position + dir) || !map->at(position + dir).passable)
			continue;

		int value = get(position + dir);

		if (value < lowest)
		{
			lowest = value;
			nextDir = dir;
		}
	}

	return nextDir;
}

void DijkstraMap::add(const sf::Vector2i& position, int value)
{
	set(position, value);
	actives.put(position, value);
}

void DijkstraMap::reset()
{
	std::fill(values.begin(), values.end(), INT_MAX);
	actives.clear();
}

void DijkstraMap::update()
{
	while (!actives.empty())
	{
		auto current = actives.get();
		int cost = current.first;
		sf::Vector2i position = current.second;

		for (auto dir : Direction::Cardinal)
		{
			if (!map->isInBounds(position + dir) || !map->at(position + dir).passable)
				continue;

			if (get(position + dir) > cost + 1)
			{
				set(position + dir, cost + 1);
				actives.put(position + dir, cost + 1);
			}
		}
	}
}