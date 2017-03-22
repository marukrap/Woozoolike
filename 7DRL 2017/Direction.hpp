#pragma once

#include <SFML/System/Vector2.hpp>

#include <array>

namespace Direction
{
	const sf::Vector2i None(0, 0);
	const sf::Vector2i N(0, -1);
	const sf::Vector2i NE(1, -1);
	const sf::Vector2i E(1, 0);
	const sf::Vector2i SE(1, 1);
	const sf::Vector2i S(0, 1);
	const sf::Vector2i SW(-1, 1);
	const sf::Vector2i W(-1, 0);
	const sf::Vector2i NW(-1, -1);

	const std::array<sf::Vector2i, 8> All = { N, NE, E, SE, S, SW, W, NW };
	const std::array<sf::Vector2i, 4> Cardinal = { N, E, S, W };
	const std::array<sf::Vector2i, 4> Diagonal = { NE, SE, SW, NW };
}