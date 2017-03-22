#pragma once

#include "Item.hpp"

class Armor : public Item
{
public:
	// using Ptr = std::unique_ptr<Armor>;

	enum Type
	{
		Helmet,
		Shield,
		Body,
		TypeCount
	};

public:
	explicit Armor(Type type);

	ItemType getItemType() const override;
	Type getArmorType() const;

	std::wstring getName(Article article = Article::None) const override;

	void changeDefense(int points);
	int getDefense() const;
	int getMaxDefense() const;

	bool apply(Actor& actor) override;

private:
	Type type;
	int defense; // durability
	int maxDefense;
};