#include "TileMap.hpp"

#include <SFML/Graphics/Texture.hpp>

TileMap::TileMap(int width, int height, const sf::Texture& texture, const sf::Vector2f& tileSize)
{
	size.x = width;
	size.y = height;

	this->texture = &texture;
	this->tileSize = tileSize;

	for (auto& layer : layers)
	{
		layer.resize(size.x * size.y * 4);

		for (int y = 0; y < size.y; ++y)
			for (int x = 0; x < size.x; ++x)
			{
				std::size_t i = (x + y * size.x) * 4;

				layer[i + 0] = sf::Vertex(sf::Vector2f((x + 0) * tileSize.x, (y + 0) * tileSize.y), sf::Color::Transparent);
				layer[i + 1] = sf::Vertex(sf::Vector2f((x + 1) * tileSize.x, (y + 0) * tileSize.y), sf::Color::Transparent);
				layer[i + 2] = sf::Vertex(sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y), sf::Color::Transparent);
				layer[i + 3] = sf::Vertex(sf::Vector2f((x + 0) * tileSize.x, (y + 1) * tileSize.y), sf::Color::Transparent);
			}
	}
}

Renderer::Type TileMap::getType() const
{
	return Renderer::Tile;
}

void TileMap::setChar(int x, int y, wchar_t ch, const sf::Color& color)
{
	if (!isInBounds(x, y))
		return;

	int tu = ch % (texture->getSize().x / static_cast<int>(tileSize.x));
	int tv = ch / (texture->getSize().x / static_cast<int>(tileSize.x));

	std::size_t i = (x + y * size.x) * 4;

	layers[TextLayer][i + 0].texCoords = sf::Vector2f((tu + 0) * tileSize.x, (tv + 0) * tileSize.y);
	layers[TextLayer][i + 1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 0) * tileSize.y);
	layers[TextLayer][i + 2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
	layers[TextLayer][i + 3].texCoords = sf::Vector2f((tu + 0) * tileSize.x, (tv + 1) * tileSize.y);

	layers[TextLayer][i + 0].color = color;
	layers[TextLayer][i + 1].color = color;
	layers[TextLayer][i + 2].color = color;
	layers[TextLayer][i + 3].color = color;
}

void TileMap::addChar(int x, int y, wchar_t ch, const sf::Color& color, const sf::Vector2f& offset, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	int tu = ch % (texture->getSize().x / static_cast<int>(tileSize.x));
	int tv = ch / (texture->getSize().x / static_cast<int>(tileSize.x));

	layers[layer].emplace_back(sf::Vector2f((x + 0) * tileSize.x + offset.x, (y + 0) * tileSize.y + offset.y), color, sf::Vector2f((tu + 0) * tileSize.x, (tv + 0) * tileSize.y));
	layers[layer].emplace_back(sf::Vector2f((x + 1) * tileSize.x + offset.x, (y + 0) * tileSize.y + offset.y), color, sf::Vector2f((tu + 1) * tileSize.x, (tv + 0) * tileSize.y));
	layers[layer].emplace_back(sf::Vector2f((x + 1) * tileSize.x + offset.x, (y + 1) * tileSize.y + offset.y), color, sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y));
	layers[layer].emplace_back(sf::Vector2f((x + 0) * tileSize.x + offset.x, (y + 1) * tileSize.y + offset.y), color, sf::Vector2f((tu + 0) * tileSize.x, (tv + 1) * tileSize.y));
}