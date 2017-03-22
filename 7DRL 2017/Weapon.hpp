#pragma once

#include "Item.hpp"

class Weapon : public Item
{
public:
	// using Ptr = std::unique_ptr<Weapon>;

	enum Type
	{
		// melee weapon
		CombatKnife,
		LaserCutlass, // EnergySword, Lightsaber

		// ranged weapon
		Pistol,
		Shotgun,
		AssaultRifle,
		SniperRifle,
		RocketLauncher,
		// Flamethrower
		// Grenade

		TypeCount
	};

public:
	explicit Weapon(Type type);

	ItemType getItemType() const override;
	Type getWeaponType() const;
	
	std::wstring getName(Article article = Article::None) const override;
	int getActorTile() const;

	int getRange() const;
	int getDamage() const;

	void changeAmmo(int amount);
	int getAmmo() const;
	int getMaxAmmo() const;

	bool takeAmmo(Weapon& weapon);

	bool apply(Actor& actor) override;

private:
	Type type;
	int range;
	int damage;
	
	// ammunition
	int ammo;
	int maxAmmo;
};