#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>

class Map;

class Fov
{
public:
	enum Algorithm
	{
		Adam,
		Bob,
	};

public:
	void setAlgorithm(Algorithm algorithm);
	Algorithm getAlgorithm() const;

	void setMap(Map& map);
	void compute(const sf::Vector2i& position, int range);

private:
	// Credit: Adam Milazzo's algorithm
	// http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html

	struct Slope
	{
		Slope(int y, int x);

		bool greater(int y, int x) const;
		bool greaterOrEqual(int y, int x) const;
		bool less(int y, int x) const;
		bool lessOrEqual(int y, int x) const;

		int x, y;
	};

	void compute(const sf::Vector2i& origin, int octant, int range, int x, Slope top, Slope bottom);

	bool blocksLight(int x, int y, int octant, sf::Vector2i origin);
	void setVisible(int x, int y, int octant, sf::Vector2i origin);

	// Credit: Bob Nystrom's algorithm
	// http://journal.stuffwithstuff.com/2015/09/07/what-the-hero-sees/

	struct Shadow
	{
		bool contains(const Shadow& projection) const;

		float start, end;
	};

	std::vector<Shadow> shadows;

	void refreshOctant(const sf::Vector2i& start, int octant, int range);

	Shadow getProjection(int col, int row);
	bool isInShadow(const Shadow& projection) const;
	bool addShadow(const Shadow& shadow);

private:
	Map* map;
	Algorithm algorithm;
};