#include "Renderer.hpp"
#include "Strings.hpp" // getLanguage

#include <SFML/Graphics/RenderTarget.hpp>

void Renderer::clear()
{
	for (int i = 0; i < LayerCount; ++i)
		clear(static_cast<Layer>(i));
}

void Renderer::clear(Layer layer)
{
	// HACK: addChar
	layers[layer].resize(size.x * size.y * 4);

	for (auto& vertex : layers[layer])
		vertex.color.a = 0;
}

void Renderer::setString(int x, int y, const std::wstring& string, const sf::Color& color)
{
	if (Strings::getLanguage() == Language::Korean)
	{
		setKoreanString(x, y, string, color);
		return;
	}

	int dx = 0, dy = 0;

	for (wchar_t ch : string)
	{
		if (ch == L'\n')
			dx = 0, ++dy;
		else
			setChar(x + dx++, y + dy, ch, color);
	}
}

void Renderer::setBox(int left, int top, int width, int height, const sf::Color& color)
{
	int right = left + width - 1;
	int bottom = top + height - 1;

	for (int y = top; y <= bottom; ++y)
		for (int x = left; x <= right; ++x)
		{
			if (width != 1 && (y == top || y == bottom))
				setChar(x, y, 0x2500, color);
			else if (height != 1 && (x == left || x == right))
				setChar(x, y, 0x2502, color);
			else
				setChar(x, y, L' ');

			// setColor(x, y, sf::Color::Transparent, Background);
			// setColor(x, y, sf::Color::Transparent, Foreground);
		}

	if (width >= 2 && height >= 2)
	{
		setChar(left, top, 0x250c, color);
		setChar(right, top, 0x2510, color);
		setChar(left, bottom, 0x2514, color);
		setChar(right, bottom, 0x2518, color);
	}
}

void Renderer::setColor(int x, int y, const sf::Color& color, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	std::size_t i = (x + y * size.x) * 4;

	layers[layer][i + 0].color = color;
	layers[layer][i + 1].color = color;
	layers[layer][i + 2].color = color;
	layers[layer][i + 3].color = color;
}

void Renderer::setColor(int left, int top, int width, int height, const sf::Color& color, Layer layer)
{
	for (int y = top; y < top + height; ++y)
		for (int x = left; x < left + width; ++x)
			setColor(x, y, color, layer);
}

void Renderer::setColorA(int x, int y, sf::Uint8 alpha, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	std::size_t i = (x + y * size.x) * 4;

	layers[layer][i + 0].color.a = alpha;
	layers[layer][i + 1].color.a = alpha;
	layers[layer][i + 2].color.a = alpha;
	layers[layer][i + 3].color.a = alpha;
}

const sf::Vector2i& Renderer::getSize() const
{
	return size;
}

const sf::Vector2f& Renderer::getTileSize() const
{
	return tileSize;
}

void Renderer::setKoreanString(int x, int y, const std::wstring& string, const sf::Color& color)
{
	// do nothing
}

bool Renderer::isInBounds(int x, int y) const
{
	return x >= 0 && x < size.x && y >= 0 && y < size.y;
}

void Renderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < LayerCount; ++i)
	{
		if (!layers[i].empty())
		{
			states.texture = (i == TextLayer ? texture : nullptr);
			target.draw(&layers[i][0], layers[i].size(), sf::Quads, states);
		}
	}
}