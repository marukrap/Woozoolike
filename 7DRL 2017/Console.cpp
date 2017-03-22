#include "Console.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

// HACK: Korean
namespace
{
	const sf::Font* koreanFont = nullptr;
	unsigned int koreanFontSize;
}
//

Console::Console(int width, int height, const sf::Font& font, unsigned int fontSize)
{
	size.x = width;
	size.y = height;
	this->font = &font;
	this->fontSize = fontSize;

	texture = &font.getTexture(fontSize);
	const_cast<sf::Texture*>(texture)->setSmooth(false);

	tileSize.x = font.getGlyph(L'A', fontSize, false).advance;
	tileSize.y = font.getLineSpacing(fontSize);
	std::cout << "Console TileSize = " << tileSize.x << " x " << tileSize.y << std::endl;

	sf::FloatRect bounds = sf::Text(L'A', font, fontSize).getLocalBounds();
	float topSpace = bounds.top;
	float bottomSpace = tileSize.y - (bounds.top + bounds.height);
	origin.y = std::floor((topSpace - bottomSpace) / 2.f);
	std::cout << "Console Origin = " << origin.x << ", " << origin.y << std::endl;

	for (auto& layer : layers)
	{
		layer.resize(width * height * 4);

		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
			{
				std::size_t i = (x + y * size.x) * 4;

				layer[i + 0] = sf::Vertex(sf::Vector2f((x + 0) * tileSize.x, (y + 0) * tileSize.y), sf::Color::Transparent);
				layer[i + 1] = sf::Vertex(sf::Vector2f((x + 1) * tileSize.x, (y + 0) * tileSize.y), sf::Color::Transparent);
				layer[i + 2] = sf::Vertex(sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y), sf::Color::Transparent);
				layer[i + 3] = sf::Vertex(sf::Vector2f((x + 0) * tileSize.x, (y + 1) * tileSize.y), sf::Color::Transparent);
			}
	}

	for (std::size_t i = 0; i < ascii.size(); ++i)
		ascii[i] = &font.getGlyph(i + 32, fontSize, false);
}

Renderer::Type Console::getType() const
{
	return Renderer::Ascii;
}

void Console::setChar(int x, int y, wchar_t ch, const sf::Color& color)
{
	if (!isInBounds(x, y))
		return;

	const sf::Glyph* glyph = nullptr;

	if (ch >= 32 && ch < 127)
		glyph = ascii[ch - 32];
	else
		glyph = &font->getGlyph(ch, fontSize, false);

	float x1 = glyph->bounds.left;
	float y1 = glyph->bounds.top + fontSize - origin.y;
	float x2 = glyph->bounds.left + glyph->bounds.width;
	float y2 = glyph->bounds.top + glyph->bounds.height + fontSize - origin.y;

	float u1 = static_cast<float>(glyph->textureRect.left);
	float v1 = static_cast<float>(glyph->textureRect.top);
	float u2 = static_cast<float>(glyph->textureRect.left + glyph->textureRect.width);
	float v2 = static_cast<float>(glyph->textureRect.top + glyph->textureRect.height);

	std::size_t i = (x + y * size.x) * 4;

	layers[TextLayer][i + 0] = sf::Vertex(sf::Vector2f(x * tileSize.x + x1, y * tileSize.y + y1), color, sf::Vector2f(u1, v1));
	layers[TextLayer][i + 1] = sf::Vertex(sf::Vector2f(x * tileSize.x + x2, y * tileSize.y + y1), color, sf::Vector2f(u2, v1));
	layers[TextLayer][i + 2] = sf::Vertex(sf::Vector2f(x * tileSize.x + x2, y * tileSize.y + y2), color, sf::Vector2f(u2, v2));
	layers[TextLayer][i + 3] = sf::Vertex(sf::Vector2f(x * tileSize.x + x1, y * tileSize.y + y2), color, sf::Vector2f(u1, v2));
}

void Console::addChar(int x, int y, wchar_t ch, const sf::Color& color, const sf::Vector2f& offset, Layer layer)
{
	if (!isInBounds(x, y))
		return;

	const sf::Glyph* glyph = nullptr;

	if (ch >= 32 && ch < 127)
		glyph = ascii[ch - 32];
	else
		glyph = &font->getGlyph(ch, fontSize, false);

	float x1 = glyph->bounds.left + offset.x;
	float y1 = glyph->bounds.top + fontSize + offset.y - origin.y;
	float x2 = glyph->bounds.left + glyph->bounds.width + offset.x;
	float y2 = glyph->bounds.top + glyph->bounds.height + fontSize + offset.y - origin.y;

	float u1 = static_cast<float>(glyph->textureRect.left);
	float v1 = static_cast<float>(glyph->textureRect.top);
	float u2 = static_cast<float>(glyph->textureRect.left + glyph->textureRect.width);
	float v2 = static_cast<float>(glyph->textureRect.top + glyph->textureRect.height);

	layers[layer].emplace_back(sf::Vector2f(x * tileSize.x + x1, y * tileSize.y + y1), color, sf::Vector2f(u1, v1));
	layers[layer].emplace_back(sf::Vector2f(x * tileSize.x + x2, y * tileSize.y + y1), color, sf::Vector2f(u2, v1));
	layers[layer].emplace_back(sf::Vector2f(x * tileSize.x + x2, y * tileSize.y + y2), color, sf::Vector2f(u2, v2));
	layers[layer].emplace_back(sf::Vector2f(x * tileSize.x + x1, y * tileSize.y + y2), color, sf::Vector2f(u1, v2));
}

// HACK: Korean
void Console::clear()
{
	Renderer::clear();

	koreanTexts.clear();
}

void Console::clear(Layer layer)
{
	Renderer::clear(layer);

	if (layer == TextLayer)
		koreanTexts.clear();
}

void Console::setKoreanFont(const sf::Font& koreanFont, unsigned int koreanFontSize)
{
	::koreanFont = &koreanFont;
	::koreanFontSize = koreanFontSize;
}

void Console::setKoreanString(int x, int y, const std::wstring& string, const sf::Color& color)
{
	koreanTexts.emplace_back(string, *koreanFont, koreanFontSize);
	koreanTexts.back().setPosition(x * tileSize.x, y * tileSize.y);
	koreanTexts.back().setFillColor(color);
}

void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Renderer::draw(target, states);

	states.transform *= getTransform();

	for (const sf::Text& text : koreanTexts)
		target.draw(text, states);
}
//