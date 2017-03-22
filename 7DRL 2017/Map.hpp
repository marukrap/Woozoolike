#pragma once

#include "Tile.hpp"

#include <SFML/System/Vector2.hpp>

#include <vector>

class Renderer;

class Map
{
public:
	Map(int width, int height);
	explicit Map(const sf::Vector2i& size);

	bool isInBounds(int x, int y) const;
	bool isInBounds(const sf::Vector2i& position) const;

	Tile& at(int x, int y);
	Tile& at(const sf::Vector2i& position);

	const Tile& at(int x, int y) const;
	const Tile& at(const sf::Vector2i& position) const;

	virtual void draw(Renderer& renderer);

public:
	const int width, height;

private:
	std::vector<Tile> tiles;
};