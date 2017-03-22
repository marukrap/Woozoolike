#include "Actor.hpp"
#include "ActorData.hpp"
#include "Vehicle.hpp"
#include "Combat.hpp"
#include "Renderer.hpp"

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Actor::Actor(ActorData& data)
	: Entity(data.glyph, data.tileNumber, Color::White)
	, data(data)
	, equipment(*this)
	, hp(1)
	, maxHp(data.hp * data.hd)
	, o2(100)
	, maxO2(100)
{
}

EntityType Actor::getEntityType() const
{
	return EntityType::Actor;
}

std::wstring Actor::getName(Article article) const
{
	return attachArticle(data.name, article);
}

bool Actor::isDestroyed() const
{
	return hp <= 0;
}

void Actor::setPosition(int x, int y)
{
	if (vehicle)
		vehicle->setPosition(x, y);

	Entity::setPosition(x, y);
}

void Actor::move(int dx, int dy)
{
	if (vehicle)
		vehicle->move(dx, dy);

	Entity::move(dx, dy);
}

void Actor::attack(Actor& actor, bool ranged)
{
	// UNDONE: combat
	Combat combat(*this, actor, ranged);
	combat.attack();
}

void Actor::takeDamage(int points)
{
	hp -= points;

	if (hp < 0)
		hp = 0;
}

void Actor::restoreHp(int points)
{
	hp += points;

	if (hp > maxHp)
		hp = maxHp;
}

void Actor::changeMaxHp(int points)
{
	maxHp += points;

	if (maxHp < 0)
		maxHp = 0;

	if (hp > maxHp)
		hp = maxHp;
}

int Actor::getHp() const
{
	return hp;
}

int Actor::getMaxHp() const
{
	return maxHp;
}

void Actor::changeO2(int oxygen)
{
	o2 += oxygen;

	if (o2 > maxO2)
		o2 = maxO2;
	else if (o2 < 0)
		o2 = 0;
}

int Actor::getO2() const
{
	return o2;
}

int Actor::getMaxO2() const
{
	return maxO2;
}

int Actor::getDamage() const
{
	if (vehicle)
		return 2; // 3

	// int damage = 1; // punch or kick

	Weapon* weapon = equipment.getCurrentWeapon();

	if (weapon)
		return weapon->getDamage();

	return data.damage;
}

void Actor::finishTurn()
{
	if (vehicle)
	{
		if (world->getLevel().getLevelType() == LevelType::Galaxy)
			vehicle->changeFuel(-1);

		else // LevelType::Planet
			vehicle->changeFuel(-2);
	}

	else
	{
		// UNDONE: consume oxygen (galaxy++)
		if (world->getLevel().getLevelType() == LevelType::Galaxy)
		{
			changeO2(-2);

			if (o2 == 9 || o2 == 10)
				world->getLog().print(Strings::get("OxygenLow"), Color::Red);

			else if (o2 == 0)
			{
				if (hp > 0)
				{
					world->getLog().print(Strings::get("OxygenEmpty"), Color::Red);
					takeDamage(1);
				}

				if (isDestroyed())
				{
					world->getLog().print(Strings::get("OxygenDied"), Color::Red);
					// log.print(L"You became the dust of the universe.", Color::Red);
					// TODO: Log -> "Esc to menu"

					return;
				}

				#ifdef _DEBUG
				world->getLog().print(L"Oxygen is full now.", Color::Cyan);
				restoreHp(1);
				changeO2(getMaxO2());
				#endif
			}
		}

		else if (world->getLevel().getLevelType() == LevelType::Planet)
		{
			// TODO: does the planet have oxygen?
			changeO2(+2);
		}

		else // LevelType::Spaceship
			changeO2(+1);
	}

	if (++regen > 20)
	{
		regen = 0;
		restoreHp(1);
	}
}

void Actor::setLastTarget(Actor* target)
{
	lastTarget = target;
}

Actor* Actor::getLastTarget() const
{
	return lastTarget;
}

void Actor::setVehicle(Vehicle* vehicle)
{
	this->vehicle = vehicle;
}

Vehicle* Actor::getVehicle() const
{
	return vehicle;
}

void Actor::updateAI()
{
	if (!isDestroyed() && ai)
		ai->update();
}

void Actor::draw(Renderer& renderer)
{
	if (isDestroyed())
		return;

	// you're in the spaceship
	if (vehicle && world->getLevel().getLevelType() != LevelType::Spaceship)
	{
		vehicle->draw(renderer);
		return;
	}

	if (renderer.getType() == Renderer::Ascii)
		renderer.setChar(position.x, position.y, glyph, color);
	
	else
	{
		if (glyph == L'o') // HACK: ooze
			renderer.addChar(position.x, position.y, tileNumber);

		else
		{
			if (world->getLevel().getLevelType() == LevelType::Spaceship || world->getLevel().getLevelType() == LevelType::Planet)
				renderer.addChar(position.x, position.y, L'_'); // shadow

			renderer.addChar(position.x, position.y, tileNumber, sf::Color::White, sf::Vector2f(0.f, -3.f));
		}

		// draw equipment
		Weapon* weapon = equipment.getCurrentWeapon();

		if (weapon)
			renderer.addChar(position.x, position.y, equipment.getCurrentWeapon()->getActorTile(), sf::Color::White, sf::Vector2f(0.f, -3.f));

		if (equipment.getArmor(Armor::Shield))
			renderer.addChar(position.x, position.y, 0xe0, sf::Color::White, sf::Vector2f(0.f, -3.f));
	}
}