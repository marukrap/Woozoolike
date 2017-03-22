#include "ActorPlayer.hpp"
#include "ActorData.hpp"
#include "Utility.hpp" // rng

ActorPlayer::ActorPlayer(ActorData& data)
	: Actor(data)
{
	hp = maxHp;

	#ifdef _DEBUG
	for (int i = 0; i < Weapon::TypeCount; ++i)
	{
		auto weapon = std::make_unique<Weapon>(static_cast<Weapon::Type>(i));
		weapon->changeAmmo(weapon->getMaxAmmo());
		equipment.weapons[i] = std::move(weapon);
	}

	equipment.currentWeapon = Weapon::Pistol;

	for (int i = 0; i < Armor::TypeCount; ++i)
	{
		auto armor = std::make_unique<Armor>(static_cast<Armor::Type>(i));
		armor->changeDefense(armor->getMaxDefense());
		equipment.armors[i] = std::move(armor);
	}
	#else
	// initialize weapons
	if (!data.weapons.empty())
	{
		Weapon::Type type = data.weapons[randomInt(data.weapons.size())];
		auto weapon = std::make_unique<Weapon>(type);
		weapon->changeAmmo(weapon->getMaxAmmo());
		equipment.weapons[type] = std::move(weapon);
		equipment.currentWeapon = type;
	}

	// initialize armor
	for (auto type : data.armors)
	{
		auto armor = std::make_unique<Armor>(type);
		armor->changeDefense(armor->getMaxDefense());
		equipment.armors[type] = std::move(armor);
	}
	#endif
}

ActorType ActorPlayer::getActorType() const
{
	return ActorType::Player;
}

bool ActorPlayer::isMarkedForRemoval() const
{
	return false;
}