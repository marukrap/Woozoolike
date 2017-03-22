#pragma once

#include "Renderer.hpp"

#include <SFML/Graphics/Text.hpp>

namespace sf
{
	class Font;
	class Glyph;
}

class Console : public Renderer
{
public:
	Console(int width, int height, const sf::Font& font, unsigned int fontSize);

	Type getType() const override;

	void setChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White) override;
	void addChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f), Layer layer = TextLayer) override;

	// HACK: Korean
	void clear() override;
	void clear(Layer layer) override;

	static void setKoreanFont(const sf::Font& koreanFont, unsigned int koreanFontSize);

private:
	void setKoreanString(int x, int y, const std::wstring& string, const sf::Color& color = sf::Color::White) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	//

private:
	const sf::Font* font = nullptr;
	unsigned int fontSize;
	
	sf::Vector2f origin;
	std::array<const sf::Glyph*, 95> ascii;
	std::vector<sf::Text> koreanTexts;
};