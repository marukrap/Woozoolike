#pragma once

#include "Renderer.hpp"

class TileMap : public Renderer
{
public:
	TileMap(int width, int height, const sf::Texture& texture, const sf::Vector2f& tileSize);
	
	Type getType() const override;

	void setChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White) override;
	void addChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f), Layer layer = TextLayer) override;
};