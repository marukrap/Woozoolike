#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <queue> // priority_queue

struct PQCompare
{
	bool operator()(const std::pair<int, sf::Vector2i>& lhs, const std::pair<int, sf::Vector2i>& rhs) const
	{
		if (lhs.first == rhs.first)
		{
			if (lhs.second.y == rhs.second.y)
				return lhs.second.x > rhs.second.x;

			return lhs.second.y > rhs.second.y;
		}

		return lhs.first > rhs.first;
	}
};

template <typename T, typename Priority>
struct PriorityQueue
{
	using PQElement = std::pair<Priority, T>;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void clear()
	{
		while (!elements.empty())
			elements.pop();
	}
	
	inline void put(T item, Priority priority)
	{
		elements.emplace(priority, item);
	}

	inline PQElement get()
	{
		PQElement top = elements.top();
		elements.pop();

		return top;
	}

	std::priority_queue<PQElement, std::vector<PQElement>, PQCompare> elements;
};

class Map;

// rough implemenetation of dijkstra map
// http://www.roguebasin.com/index.php?title=The_Incredible_Power_of_Dijkstra_Maps

class DijkstraMap
{
public:
	void setMap(Map& map);

	void set(int x, int y, int value);
	void set(const sf::Vector2i& position, int value);

	int get(int x, int y) const;
	int get(const sf::Vector2i& position) const;

	sf::Vector2i getNextDir(const sf::Vector2i& position) const;

	void add(const sf::Vector2i& position, int value);

	void reset();
	void update();

private:
	Map* map = nullptr;
	sf::Vector2i size;
	std::vector<int> values;
	PriorityQueue<sf::Vector2i, int> actives;
};