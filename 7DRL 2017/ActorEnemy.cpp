#include "ActorEnemy.hpp"
#include "ActorData.hpp"
#include "Utility.hpp" // rng

ActorEnemy::ActorEnemy(ActorData& data)
	: Actor(data)
{
	hp = rollDice(data.hd, data.hp);

	// initialize weapons
	if (!data.weapons.empty())
	{
		Weapon::Type type = data.weapons[randomInt(data.weapons.size())];
		auto weapon = std::make_unique<Weapon>(type);
		equipment.weapons[type] = std::move(weapon);
		equipment.currentWeapon = type;
	}

	// initialize armor
	for (auto type : data.armors)
	{
		if (randomInt(2) == 0)
		{
			auto armor = std::make_unique<Armor>(type);
			equipment.armors[type] = std::move(armor);
		}
	}

	ai = std::make_unique<AI>(*this);
}

ActorType ActorEnemy::getActorType() const
{
	return ActorType::Enemy;
}