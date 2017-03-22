#include "Equipment.hpp"
#include "Actor.hpp"
#include "Utility.hpp"
#include "Direction.hpp"

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Equipment::Equipment(Actor& actor)
	: actor(actor)
{
	std::fill(weapons.begin(), weapons.end(), nullptr);
	std::fill(armors.begin(), armors.end(), nullptr);
}

Weapon* Equipment::getCurrentWeapon() const
{
	if (currentWeapon < 0)
		return nullptr;
	
	return getWeapon(static_cast<Weapon::Type>(currentWeapon));
}

void Equipment::dropCurrentWeapon()
{
	if (currentWeapon >= 0)
	{
		if (world->getLevel().getEntity(actor.getPosition(), Level::ItemLayer))
		{
			if (actor.getActorType() == ActorType::Player)
				world->getLog().print(Strings::get("DropFail"));

			else // when an enemy dies
			{
				std::vector<sf::Vector2i> positions;

				for (auto dir : Direction::All)
				{
					sf::Vector2i position = actor.getPosition() + dir;

					if (world->getLevel().at(position).passable && !world->getLevel().getEntity(position, Level::ItemLayer))
						positions.emplace_back(position);
				}

				if (!positions.empty())
				{
					sf::Vector2i position = positions[randomInt(positions.size())];
					
					world->getLog().print(Strings::get("DropWeapon"), sf::Color::White, { actor.getName(Article::The), weapons[currentWeapon]->getName(Article::A) });

					weapons[currentWeapon]->setPosition(position);
					world->getLevel().attachEntity(std::move(weapons[currentWeapon]), Level::ItemLayer);

					// currentWeapon = -1;
				}
			}
		}

		else
			setWeapon(static_cast<Weapon::Type>(currentWeapon), nullptr);
	}
}

void Equipment::selectWeapon(int index)
{
	if (index < 0) // bare hands
		currentWeapon = index;

	else if (currentWeapon != index)
	{
		Weapon* weapon = getWeapon(static_cast<Weapon::Type>(index));

		if (weapon)
			currentWeapon = index;

		else
		{
			// you don't have this kind of weapon
		}
	}
}

void Equipment::selectPrevWeapon()
{
	// currentWeapon = (currentWeapon + Weapon::TypeCount - 1) % Weapon::TypeCount;
	// selectWeapon(currentWeapon);

	if (currentWeapon == -1)
		currentWeapon = Weapon::TypeCount;
		
	for (int i = currentWeapon - 1; i >= 0; --i)
	{
		if (weapons[i])
		{
			selectWeapon(i);
			return;
		}
	}

	selectWeapon(-1);
}

void Equipment::selectNextWeapon()
{
	// currentWeapon = (currentWeapon + 1) % Weapon::TypeCount;
	// selectWeapon(currentWeapon);

	for (int i = currentWeapon + 1; i < Weapon::TypeCount; ++i)
	{
		if (weapons[i])
		{
			selectWeapon(i);
			return;
		}
	}

	selectWeapon(-1);
}

Weapon* Equipment::getWeapon(Weapon::Type type) const
{
	// REMOVE: dynamic_cast
	return dynamic_cast<Weapon*>(weapons[type].get());
}

void Equipment::setWeapon(Weapon::Type type, Weapon* weapon)
{
	if (weapons[type])
	{
		if (weapon)
		{
			if (weapon->getAmmo() > 0)
			{
				// REMOVE: dynamic_cast
				Weapon* current = dynamic_cast<Weapon*>(weapons[type].get());

				if (current->takeAmmo(*weapon))
					world->getLog().print(Strings::get("TakeAmmo"), Color::Yellow, { current->getName() });
				else
					world->getLog().print(Strings::get("AmmoFull"));
			}

			else // empty ranged weapon or melee weapon
				world->getLog().print(Strings::get("AlreadyHave"));

			return;
		}

		else
		{
			// HACK: when an enemy dies
			if (actor.isDestroyed())
				world->getLog().print(Strings::get("DropWeapon"), sf::Color::White, { actor.getName(Article::The), weapons[currentWeapon]->getName(Article::A) });
			else
				// TODO: you discarded your pistol.
				world->getLog().print(Strings::get("DiscardWeapon"), sf::Color::White, { actor.getName(Article::The), weapons[currentWeapon]->getName(Article::A) });
		}

		// unequip and drop old weapon
		weapons[type]->setPosition(actor.getPosition());
		world->getLevel().attachEntity(std::move(weapons[type]), Level::ItemLayer);
	}

	if (weapon)
	{
		world->getLog().print(Strings::get("PickUpWeapon"), Color::Yellow, { weapon->getName(Article::A) });

		weapons[type] = world->getLevel().detachEntity(*weapon, Level::ItemLayer);
		currentWeapon = type;
	}

	else
	{
		weapons[type] = nullptr;
		currentWeapon = -1;
	}
}

Armor* Equipment::getArmor(Armor::Type type) const
{
	// HACK: shield
	if (type == Armor::Shield && currentWeapon > Weapon::Pistol)
		return nullptr;

	// REMOVE: dynamic_cast
	return dynamic_cast<Armor*>(armors[type].get());
}

void Equipment::setArmor(Armor::Type type, Armor* armor)
{
	if (!armor)
	{
		// discard armor
		armors[type] = nullptr;
		return;
	}

	if (armors[type])
	{
		world->getLog().print(Strings::get("DropArmor"), sf::Color::White, { actor.getName(Article::The), armors[type]->getName(Article::A) });

		// unequip and drop old armor
		armors[type]->setPosition(actor.getPosition());
		world->getLevel().attachEntity(std::move(armors[type]), Level::ItemLayer);
	}

	armors[type] = world->getLevel().detachEntity(*armor, Level::ItemLayer);
}