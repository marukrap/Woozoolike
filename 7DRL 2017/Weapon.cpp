#include "Weapon.hpp"
#include "Actor.hpp"
#include "Utility.hpp" // rng
#include "Strings.hpp"
#include "Color.hpp"

#include <cassert>

Weapon::Weapon(Type type)
	: Item(L')', L')', Color::Yellow)
	, type(type)
	, damage(0)
	, range(0)
	, ammo(0)
	, maxAmmo(0)
{
	// TODO: add durability for melee weapon (instead of ammo)

	// TODO: make weapon data table
	switch (type)
	{
	case CombatKnife:
		glyph = L'(';
		tileNumber = L'1';
		range = 1;
		damage = 3;
		break;

	case LaserCutlass:
		glyph = L'(';
		tileNumber = L'8';
		range = 1;
		damage = 3; // 3x2
		break;

	case Pistol:
		glyph = L')';
		tileNumber = L'2';
		range = 5;
		damage = 2;
		ammo = randomInt(9, 15);
		maxAmmo = 15;
		break;

	case Shotgun:
		glyph = L')';
		tileNumber = L'3';
		range = 4;
		damage = 4; // 3-6 (it depends on the distance)
		ammo = randomInt(3, 7);
		maxAmmo = 7;
		break;

	case AssaultRifle:
		glyph = L')';
		tileNumber = L'4';
		range = 7;
		damage = 2; // 2x3
		ammo = randomInt(5, 10) * 3;
		maxAmmo = 10 * 3;
		break;

	case SniperRifle:
		glyph = L')';
		tileNumber = L'5';
		range = 10;
		damage = 4;
		ammo = randomInt(3, 7);
		maxAmmo = 7;
		break;

	case RocketLauncher:
		glyph = L')';
		tileNumber = L'6';
		range = 6;
		damage = 8;
		ammo = randomInt(1, 3);
		maxAmmo = 3;
		break;
	}
}

ItemType Weapon::getItemType() const
{
	return ItemType::Weapon;
}

Weapon::Type Weapon::getWeaponType() const
{
	return type;
}

std::wstring Weapon::getName(Article article) const
{
	std::wstring name;

	switch (type)
	{
	case CombatKnife:		name = Strings::get("CombatKnife"); break;
	case LaserCutlass:		name = Strings::get("LaserCutlass"); break;
	case Pistol:			name = Strings::get("Pistol"); break;
	case Shotgun:			name = Strings::get("Shotgun"); break;
	case AssaultRifle:		name = Strings::get("AssaultRifle"); break;
	case SniperRifle:		name = Strings::get("SniperRifle"); break;
	case RocketLauncher:	name = Strings::get("RocketLauncher"); break;
	}

	// if (type >= Pistol)
		// name += L" (x" + std::to_wstring(ammo) + L")";

	return attachArticle(name, article);
}

int Weapon::getActorTile() const
{
	// HACK:
	return 0xe0 + (tileNumber - L'0');
}

int Weapon::getRange() const
{
	return range;
}

int Weapon::getDamage() const
{
	return damage;
}

void Weapon::changeAmmo(int amount)
{
	ammo += amount;

	if (ammo > maxAmmo)
		ammo = maxAmmo;

	assert(ammo >= 0);
}

int Weapon::getAmmo() const
{
	return ammo;
}

int Weapon::getMaxAmmo() const
{
	return maxAmmo;
}

bool Weapon::takeAmmo(Weapon& weapon)
{
	if (ammo == maxAmmo)
		return false;

	int remainder = 0;

	ammo += weapon.ammo;

	if (ammo > maxAmmo)
	{
		remainder = ammo - maxAmmo;
		ammo = maxAmmo;
	}

	weapon.ammo = remainder;

	return true;
}

bool Weapon::apply(Actor& actor)
{
	actor.equipment.setWeapon(type, this);

	// NOTE: Log?

	return false;
}