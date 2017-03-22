#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>
#include <array>

class Renderer : public sf::Drawable, public sf::Transformable
{
public:
	enum Type
	{
		Ascii,
		Tile,
		TypeCount
	};

	enum Layer
	{
		Background,
		TextLayer,
		Foreground,
		LayerCount
	};

public:
	virtual ~Renderer() = default;

	virtual Type getType() const = 0;

	virtual void clear();
	virtual void clear(Layer layer);

	virtual void setChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White) = 0;
	virtual void addChar(int x, int y, wchar_t ch, const sf::Color& color = sf::Color::White, const sf::Vector2f& offset = sf::Vector2f(0.f, 0.f), Layer layer = TextLayer) = 0;

	void setString(int x, int y, const std::wstring& string, const sf::Color& color = sf::Color::White);
	void setBox(int left, int top, int width, int height, const sf::Color& color = sf::Color::White);

	void setColor(int x, int y, const sf::Color& color, Layer layer = Background);
	void setColor(int left, int top, int width, int height, const sf::Color& color, Layer layer = Background);

	void setColorA(int x, int y, sf::Uint8 alpha, Layer layer = TextLayer);

	const sf::Vector2i& getSize() const;
	const sf::Vector2f& getTileSize() const;

	virtual void setKoreanString(int x, int y, const std::wstring& string, const sf::Color& color = sf::Color::White);

protected:
	bool isInBounds(int x, int y) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	const sf::Texture* texture = nullptr;

	sf::Vector2i size;
	sf::Vector2f tileSize;
	std::array<std::vector<sf::Vertex>, LayerCount> layers;
};