#pragma once

#include "Weapon.hpp"
#include "Armor.hpp"

#include <array>

class Equipment : public GameObject
{
public:
	/*
	enum Slot
	{
		MainHand,
		OffHand,
		Head,
		Chest,
		SlotCount
	};
	*/

public:
	explicit Equipment(Actor& owner);

	Weapon* getCurrentWeapon() const;
	void dropCurrentWeapon();

	void selectWeapon(int index);
	void selectPrevWeapon();
	void selectNextWeapon();

	Weapon* getWeapon(Weapon::Type type) const;
	void setWeapon(Weapon::Type type, Weapon* weapon);

	Armor* getArmor(Armor::Type type) const;
	void setArmor(Armor::Type type, Armor* armor);

private:
	Actor& actor;
	std::array<Entity::Ptr, Weapon::TypeCount> weapons;
	std::array<Entity::Ptr, Armor::TypeCount> armors;

	int currentWeapon = -1;

	// REMOVE: friend
	friend class ActorPlayer;
	friend class ActorEnemy;
};