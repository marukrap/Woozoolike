#pragma once

#include "GameObject.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <string>
#include <memory> // unique_ptr

enum class Article
{
	None,
	A, // An
	The,
};

enum class EntityType
{
	Actor,
	Item,
	Vehicle, // Spaceship
	Planet,
	Projectile,
};

class Renderer;
class Level;

class Entity : public GameObject
{
public:
	using Ptr = std::unique_ptr<Entity>;

public:
	Entity(wchar_t glyph, int tileNumber, const sf::Color& color = sf::Color::White);

	virtual EntityType getEntityType() const = 0;

	virtual bool isDestroyed() const;
	virtual bool isMarkedForRemoval() const;

	// void setGlyph(wchar_t glyph);
	wchar_t getGlyph() const;

	// void setTileNumber(int tileNumber);
	int getTileNumber() const;

	// void setColor(const sf::Color& color);
	const sf::Color& getColor() const;

	virtual std::wstring getName(Article article = Article::None) const = 0;
	// virtual const std::wstring& getDescription() const = 0;

	virtual void setPosition(int x, int y);
	void setPosition(const sf::Vector2i& position);
	const sf::Vector2i& getPosition() const;

	virtual void move(int dx, int dy);
	void move(const sf::Vector2i& dir);

	// used for vehicle and planet
	virtual void attachLevel(std::unique_ptr<Level> level);
	virtual Level* getLevel() const;

	virtual void draw(Renderer& renderer);

protected:
	std::wstring attachArticle(const std::wstring& name, Article article) const;

protected:
	wchar_t glyph;
	int tileNumber;
	sf::Color color;
	sf::Vector2i position;
};