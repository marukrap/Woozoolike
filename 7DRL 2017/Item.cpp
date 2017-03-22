#include "Item.hpp"
#include "Renderer.hpp"

#include "World.hpp"

EntityType Item::getEntityType() const
{
	return EntityType::Item;
}

void Item::draw(Renderer& renderer)
{
	if (renderer.getType() == Renderer::Ascii)
		renderer.setChar(position.x, position.y, glyph, color);

	else // Renderer::Tile
	{
		sf::Color tileColor = sf::Color::White;

		// HACK: fov
		if (!world->getLevel().at(position).visible)
			tileColor.a = 51;

		renderer.addChar(position.x, position.y, tileNumber, tileColor);
	}
}