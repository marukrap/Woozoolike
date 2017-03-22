#include "AI.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "Strings.hpp"
#include "Utility.hpp" // length

#include <iostream>

AI::AI(Actor& actor)
	: actor(actor)
{
}

void AI::update()
{
	bool canSeeTarget = world->getLevel().at(actor.getPosition()).visible;
	Actor* target = actor.getLastTarget();

	if (target)
	{
		if (!canSeeTarget)
		{
			// the actor loses interest in the target
			if (morale <= 0)
			{
				// TODO: return to the original position
				target = nullptr;
			}

			else
				--morale;
		}

		else
		{
			if (morale < 1)
				morale = 10;

			else if (morale < 20)
				++morale;
		}

		// TODO: if the target attacks the actor, morale++

		if (target)
		{
			int distance = length(actor.getPosition() - target->getPosition());
			Weapon* weapon = actor.equipment.getCurrentWeapon();

			if (canSeeTarget && weapon && distance <= weapon->getRange())
			{
				if (weapon->getRange() == 1)
					actor.attack(*target, false);

				else
				{
					// discard equipped weapon
					if (weapon->getAmmo() == 0)
						actor.equipment.dropCurrentWeapon();

					else
					{
						int bullets = 1;

						if (weapon->getWeaponType() == Weapon::AssaultRifle)
							bullets = weapon->getAmmo() >= 3 ? 3 : weapon->getAmmo();

						world->getProjectileQueue().add(actor, target->getPosition(), bullets);
						weapon->changeAmmo(-bullets);
					}
				}
			}

			else if (distance == 1)
				actor.attack(*target, false);

			else
			{
				// walk toward to the target
				sf::Vector2i position = actor.getPosition();
				sf::Vector2i nextDir = world->getDijkstraMap().getNextDir(position);

				if (nextDir.x == 0 && nextDir.y == 0)
					std::wcout << actor.getName(Article::The) << L" can't find path.\n";

				else
				{
					// TODO: attack if the entity is an enemy
					if (world->getLevel().getEntity(position + nextDir, Level::ActorLayer))
						std::wcout << actor.getName(Article::The) << L" is blocked by someone.\n";
					else // do move
						actor.move(nextDir);
				}
			}
		}

		// TODO: run away
	}

	else // if (!target)
	{
		if (canSeeTarget)
		{
			// the actor notices the player
			target = &world->getPlayerActor();
			morale = 10;

			world->getLog().print(Strings::get("Notice"), sf::Color::White, { actor.getName(Article::The), target->getName(Article::The) });
		}

		// else
			// TODO: wonder
	}

	actor.setLastTarget(target);
}