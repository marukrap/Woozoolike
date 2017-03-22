#pragma once

#include "Entity.hpp"

enum class ItemType
{
	Weapon,
	Armor,
	Medkit, // Consumable
	// Food, // SoylentGreen
	Amulet,
};

class Actor;

class Item : public Entity
{
public:
	using Ptr = std::unique_ptr<Item>;

public:
	using Entity::Entity;

	EntityType getEntityType() const override;
	virtual ItemType getItemType() const = 0;

	virtual bool apply(Actor& actor) = 0;
	
	void draw(Renderer& renderer) override;

private:
};