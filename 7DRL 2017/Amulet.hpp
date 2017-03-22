#pragma once

#include "Item.hpp"

// a piece of the Yendor amulet
class Amulet : public Item
{
public:
	Amulet(wchar_t glyph, int tileNumber);

	ItemType getItemType() const override;

	std::wstring getName(Article article = Article::None) const override;

	bool apply(Actor& actor) override;

private:
};