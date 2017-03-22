#include "Entity.hpp"
#include "Renderer.hpp"
#include "Strings.hpp"
#include "Level.hpp"

Entity::Entity(wchar_t glyph, int tileNumber, const sf::Color& color)
	: glyph(glyph)
	, tileNumber(tileNumber)
	, color(color)
{
}

bool Entity::isDestroyed() const
{
	return false;
}

bool Entity::isMarkedForRemoval() const
{
	return isDestroyed();
}

/*
void Entity::setGlyph(wchar_t glyph)
{
	this->glyph = glyph;
}
*/

wchar_t Entity::getGlyph() const
{
	return glyph;
}

/*
void Entity::setTileNumber(int tileNumber)
{
	this->tileNumber = tileNumber;
}
*/

int Entity::getTileNumber() const
{
	return tileNumber;
}

/*
void Entity::setColor(const sf::Color& color)
{
	this->color = color;
}
*/

const sf::Color& Entity::getColor() const
{
	return color;
}

void Entity::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

void Entity::setPosition(const sf::Vector2i& position)
{
	setPosition(position.x, position.y);
}

const sf::Vector2i& Entity::getPosition() const
{
	return position;
}

void Entity::move(int dx, int dy)
{
	position.x += dx;
	position.y += dy;
}

void Entity::move(const sf::Vector2i& dir)
{
	move(dir.x, dir.y);
}

void Entity::attachLevel(std::unique_ptr<Level> level)
{
}

Level* Entity::getLevel() const
{
	return nullptr;
}

void Entity::draw(Renderer& renderer)
{
	if (renderer.getType() == Renderer::Ascii)
		renderer.setChar(position.x, position.y, glyph, color);
	else // Renderer::Tile
		renderer.addChar(position.x, position.y, tileNumber);
}

std::wstring Entity::attachArticle(const std::wstring& name, Article article) const
{
	if (Strings::getLanguage() != Language::English) // Korean
		return name;

	std::wstring string = name;

	if (article == Article::A)
	{
		if (std::wstring(L"aeiou").find(string[0]) != std::wstring::npos)
			string = L"an " + string;
		else
			string = L"a " + string;
	}

	else if (article == Article::The)
	{
		if (string != L"you" && string != L"something")
			string = L"the " + string;
	}

	return string;
}