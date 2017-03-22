#include "Combat.hpp"
#include "Actor.hpp"
#include "Utility.hpp" // length

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Combat::Combat(Actor& attacker, Actor& defender, bool ranged)
	: attacker(attacker)
	, defender(defender)
	, ranged(ranged)
	, distance(length(attacker.getPosition() - defender.getPosition()))
{
	attacker.lastTarget = &defender;

	weapon = attacker.equipment.getCurrentWeapon();
	defenderWeapon = defender.equipment.getCurrentWeapon();

	shield = defender.equipment.getArmor(Armor::Shield);
	helmet = defender.equipment.getArmor(Armor::Helmet);
	body = defender.equipment.getArmor(Armor::Body);

	if (attacker.vehicle)
		weapon = nullptr; // missile

	if (defender.vehicle)
		shield = helmet = body = nullptr;
}

void Combat::attack()
{
	if (defender.isDestroyed())
		return;

	if (hit())
		damage();

	// UNDONE: LaserCutlass
	if (!defender.isDestroyed() && weapon && weapon->getWeaponType() == Weapon::LaserCutlass)
	{
		weapon = attacker.equipment.getCurrentWeapon();
		defenderWeapon = defender.equipment.getCurrentWeapon();

		shield = defender.equipment.getArmor(Armor::Shield);
		helmet = defender.equipment.getArmor(Armor::Helmet);
		body = defender.equipment.getArmor(Armor::Body);

		if (attacker.vehicle)
			weapon = nullptr; // missile

		if (defender.vehicle)
			shield = helmet = body = nullptr;

		if (hit())
			damage();
	}
}

bool Combat::hit()
{
	// base hit
	// 1-3: 95
	// 4-6: 85
	// 7- : 75
	int hitRate = 85;

	if (distance <= 3)
		hitRate += 10;
	else if (distance >= 7)
		hitRate -= 10;

	// sniper rifle
	// 1-3: 85
	// 4-6: 100
	// 7- : 90
	if (weapon && weapon->getWeaponType() == Weapon::SniperRifle)
	{
		if (distance > 3)
			hitRate += 15;
	}

	// bare hands or melee weapon
	if (!defenderWeapon || defenderWeapon->getRange() == 1)
	{
		hitRate -= 10;

		// TODO: Walking -5, Running -10
	}

	if (helmet && ranged && avoid(*helmet))
		return false;

	if (shield && ranged && avoid(*shield))
		return false;

	if (randomInt(100) < hitRate)
		return true;

	std::wstring verb = Strings::get(attacker.getActorType() == ActorType::Player ? "Miss" : "Misses");
	world->getLog().print(Strings::get("Attack"), sf::Color::White, { attacker.getName(Article::The), verb, defender.getName(Article::The) });

	return false;
}

void Combat::damage()
{
	std::wstring name = attacker.getName(Article::The);
	std::wstring defenderName = defender.getName(Article::The);
	std::wstring verb;
	bool thirdPerson = attacker.getActorType() != ActorType::Player;
	sf::Color color = attacker.getActorType() == ActorType::Player ? Color::Blue : Color::Red;

	int damage = attacker.getDamage();

	if (weapon)
	{
		// melee attack with a ranged weapon
		if (weapon->getRange() > 1 && !ranged)
			damage = 1;

		else if (weapon->getWeaponType() == Weapon::Shotgun)
		{
			switch (distance)
			{
			case 1: damage += 2; break; // TODO: push one space (knock back)
			case 2: damage += 1; break;
			case 4: damage -= 1; break; // max range
			}
		}
	}

	if (shield && block(*shield, damage))
		return;

	if (body && block(*body, damage))
		return;

	// do damage
	defender.takeDamage(damage);

	if (ranged)
	{
		// name = Strings::get("TheBullet");
		verb = Strings::get("RangedHits");
		thirdPerson = true;

		if (attacker.vehicle)
			name = Strings::get("TheMissile");

		else if (weapon->getWeaponType() == Weapon::RocketLauncher)
			name = Strings::get("TheExplosion");

		else if (weapon->getWeaponType() == Weapon::Shotgun)
		{
			name = Strings::get("TheBullets");
			verb = Strings::get("RangedHit");
			thirdPerson = false;
		}

		else
			name = Strings::get("TheBullet");
	}

	else if (weapon)
	{
		if (weapon->getRange() > 1)
			verb = Strings::get(thirdPerson ? "Hits" : "Hit"); // or knock back

		else
		{
			switch (randomInt(3))
			{
			case 0: verb = Strings::get(thirdPerson ? "Slashes" : "Slash"); break;
			case 1: verb = Strings::get(thirdPerson ? "Slices" : "Slice"); break;
			case 2: verb = Strings::get(thirdPerson ? "CutsOff" : "CutOff"); break;
			}
		}
	}

	else // bare hands
	{
		// punch, kick, bite, etc.
		verb = Strings::get(thirdPerson ? "Hits" : "Hit");
	}
	
	#ifdef _DEBUG // if (damage > 0)
		world->getLog().print(Strings::get(ranged ? "RangedAttack2" : "Attack2"), color, { name, verb, defenderName, std::to_wstring(damage) });
	#else // else
		world->getLog().print(Strings::get(ranged ? "RangedAttack" : "Attack"), color, { name, verb, defenderName });
	#endif

	if (defender.isDestroyed())
	{
		if (defender.getActorType() == ActorType::Player)
		{
			world->getLog().print(Strings::get("YouDie"), color);
			// TODO: Log -> "Esc to menu"

			#ifdef _DEBUG
			world->getLog().print(L"Your health is full now.", Color::Cyan);
			defender.restoreHp(defender.getMaxHp());
			#endif
		}

		else
		{
			world->getLog().print(Strings::get("Kill"), color, { defenderName });

			// drop equipped weapon
			defender.equipment.dropCurrentWeapon();
		}

		attacker.lastTarget = nullptr;

		// UNDONE: bloodstain, 적(몬스터) 종류에 따른 피, 체력이 절반 이하로 떨어질 경우에도 흘림
		Tile& tile = world->getLevel().at(defender.getPosition());

		if (tile.blood < 0)
			tile.blood = 0xf8 + randomInt(3);
		else
			tile.blood = 0xfb;

		tile.color = Color::Red;

		// TODO: if all enemies have died
			// Log -> the ship/planet is peaceful now.
	}
}

bool Combat::avoid(Armor& armor)
{
	int chance = 0;

	if (armor.getArmorType() == Armor::Helmet)
		chance = 10;
	else // if (armor.getArmorType() == Armor::Shield)
		chance = 15;

	if (randomInt(100) < chance)
	{
		armor.changeDefense(-1);

		std::wstring name = makePossessive(defender.getName(Article::The), armor.getName());

		if (armor.getDefense() <= 0)
		{
			// destroyed
			sf::Color color = defender.getActorType() == ActorType::Player ? Color::Red : Color::Blue;
			world->getLog().print(Strings::get("Destroyed"), color, { name });

			defender.equipment.setArmor(armor.getArmorType(), nullptr);
		}

		else
		{
			// TODO: the bullet -> getProjectileName()
			if (armor.getArmorType() == Armor::Helmet)
				world->getLog().print(Strings::get("Graze"), sf::Color::White, { Strings::get("TheBullet"), name });
			else // if (armor.getArmorType() == Armor::Shield)
				world->getLog().print(Strings::get("GlanceOff"), sf::Color::White, { Strings::get("TheBullet"), name });
		}

		return true;
	}

	return false;
}

bool Combat::block(Armor& armor, int& damage)
{
	damage -= damage / 2;

	armor.changeDefense(-1);

	std::wstring name = makePossessive(defender.getName(Article::The), armor.getName());

	if (armor.getDefense() <= 0)
	{
		// destroyed
		sf::Color color = defender.getActorType() == ActorType::Player ? Color::Red : Color::Blue;
		world->getLog().print(Strings::get("Destroyed"), color, { name });

		defender.equipment.setArmor(armor.getArmorType(), nullptr);
	}

	else if (damage == 0)
	{
		// blocked all damage
		world->getLog().print(Strings::get("Block"), sf::Color::White, { name });
		
		return true;
	}

	return false;
}

std::wstring Combat::makePossessive(const std::wstring& subject, const std::wstring& noun)
{
	std::wstring result;

	if (Strings::getLanguage() == Language::English)
	{
		if (subject == L"you")
			result = L"your ";
		else
			result = subject + L"'s ";
	}

	else if (Strings::getLanguage() == Language::Korean)
		result = subject + L"의 ";

	return result + noun;
}