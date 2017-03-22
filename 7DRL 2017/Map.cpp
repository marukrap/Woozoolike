#include "Map.hpp"
#include "Renderer.hpp"

Map::Map(int width, int height)
	: width(width)
	, height(height)
	, tiles(width * height)
{
}

Map::Map(const sf::Vector2i& size)
	: width(size.x)
	, height(size.y)
	, tiles(size.x * size.y)
{
}

bool Map::isInBounds(int x, int y) const
{
	return x >= 0 && x < width && y >= 0 && y < height;
}

bool Map::isInBounds(const sf::Vector2i& position) const
{
	return isInBounds(position.x, position.y);
}

Tile& Map::at(int x, int y)
{
	return tiles[x + y * width];
}

Tile& Map::at(const sf::Vector2i& position)
{
	return at(position.x, position.y);
}

const Tile& Map::at(int x, int y) const
{
	return tiles[x + y * width];
}

const Tile& Map::at(const sf::Vector2i& position) const
{
	return at(position.x, position.y);
}

void Map::draw(Renderer& renderer)
{
	if (renderer.getType() == Renderer::Ascii)
	{
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				Tile& tile = at(x, y);
				sf::Color color = tile.color;

				if (!tile.visible)
				{
					if (!tile.explored)
						continue;
					else
						color.a = 51; // color.rgb / 5
				}

				renderer.setChar(x, y, tile.glyph, color);
			}
	}

	else // Renderer::Tile
	{
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				Tile& tile = at(x, y);
				sf::Color color = sf::Color::White;

				if (!tile.visible)
				{
					if (!tile.explored)
						continue;
					else
						color.a = 51; // color.rgb / 5
				}

				renderer.setChar(x, y, tile.tileNumber, color);

				if (tile.blood >= 0)
					renderer.addChar(x, y, tile.blood, color);
			}
	}
}