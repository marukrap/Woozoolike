#include "Amulet.hpp"
#include "Actor.hpp" // amulet

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Amulet::Amulet(wchar_t glyph, int tileNumber)
	: Item(glyph, tileNumber, Color::Yellow)
{
}

ItemType Amulet::getItemType() const
{
	return ItemType::Amulet;
}

std::wstring Amulet::getName(Article article) const
{
	return attachArticle(Strings::get("AmuletPiece"), article);
}

bool Amulet::apply(Actor& actor)
{
	// UNDONE: amulet, victory - globals?

	if (actor.amulet >= 8)
	{
		// you don't need a piece of the amulet anymore
		return false;
	}

	++actor.amulet;

	if (actor.amulet == 8)
	{
		world->getLog().print(Strings::get("AmuletAll"), Color::Cyan);
		world->getLog().print(Strings::get("AmuletAll2"), Color::Yellow);
	}

	else if (actor.amulet == 3)
	{
		world->getLog().print(Strings::get("AmuletThree"), Color::Cyan);
		world->getLog().print(Strings::get("AmuletThree2"), Color::Yellow);
	}

	else if (actor.amulet == 1)
		world->getLog().print(Strings::get("AmuletGet"), Color::Cyan);

	else
		world->getLog().print(Strings::get("AmuletGet2"), Color::Cyan, { std::to_wstring(actor.amulet) });

	return true;
}