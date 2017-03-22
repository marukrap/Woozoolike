#pragma once

#include <SFML/Graphics/Color.hpp>

struct Tile
{
	enum Type : wchar_t
	{
		Unused		= L'\0',
		Floor		= L'.',
		Wall		= L'#',
		Object		= L'&',
		ClosedDoor	= L'+', // бс
		OpenDoor	= L'-', // бр
		UpStairs	= L'<',
		DownStairs	= L'>',
	};

	wchar_t glyph	= Unused; // ASCII or Unicode glyph
	int tileNumber	= -1;
	sf::Color color	= sf::Color::White;
	bool passable	= false;
	bool transparent = false;
	bool visible	= false;
	bool explored	= false;
	
	// HACK: blood tile number
	int blood		= -1;
};