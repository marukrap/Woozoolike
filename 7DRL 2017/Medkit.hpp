#pragma once

#include "Item.hpp"

class Medkit : public Item
{
public:
	Medkit();

	ItemType getItemType() const override;

	std::wstring getName(Article article = Article::None) const override;

	bool apply(Actor& actor) override;
};