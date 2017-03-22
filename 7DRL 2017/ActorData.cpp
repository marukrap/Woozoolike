#include "ActorData.hpp"
#include "Strings.hpp"
#include "Color.hpp"

std::vector<ActorData> initializeActorData()
{
	std::vector<ActorData> data;

	data.push_back({ L'@', L'@', 10, 1, 1, Color::White, Strings::get("You"), { Weapon::Pistol }, { Armor::Body } });
	data.push_back({ L'a', L'a',  3, 1, 1, Color::White, Strings::get("Alien"), { Weapon::CombatKnife, Weapon::Pistol }, {} });
	data.push_back({ L'b', L'b',  3, 2, 1, Color::White, Strings::get("Birdian"), { Weapon::CombatKnife, Weapon::Pistol, Weapon::AssaultRifle }, { Armor::Shield } });
	data.push_back({ L'c', L'c',  4, 2, 2, Color::White, Strings::get("Clay"), { Weapon::CombatKnife, Weapon::Pistol, Weapon::AssaultRifle }, { Armor::Body } });
	/* TODO: */ data.push_back({ L'd', L'd',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'e', L'e',  3, 3, 1, Color::White, Strings::get("EvolvedMutant"), { Weapon::CombatKnife, Weapon::AssaultRifle }, { Armor::Shield } });
	data.push_back({ L'f', L'f',  1, 1, 1, Color::White, Strings::get("Folyp"), {}, {} });
	data.push_back({ L'g', L'g',  3, 2, 1, Color::White, Strings::get("RiotPolice"), { Weapon::AssaultRifle, Weapon::SniperRifle, Weapon::RocketLauncher, }, { Armor::Helmet, Armor::Shield, Armor::Body } });
	/* TODO: */ data.push_back({ L'h', L'h',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'i', L'i',  4, 2, 2, Color::White, Strings::get("Ilyth"), { Weapon::CombatKnife, Weapon::Pistol, Weapon::AssaultRifle, Weapon::RocketLauncher }, { Armor::Helmet, Armor::Shield, Armor::Body } });
	/* TODO: */ data.push_back({ L'j', L'j',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	/* TODO: */ data.push_back({ L'k', L'k',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	/* TODO: */ data.push_back({ L'l', L'l',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'm', L'm',  3, 1, 1, Color::White, Strings::get("Mutant"), {}, {} });
	/* TODO: */ data.push_back({ L'n', L'n',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'o', L'o',  3, 1, 1, Color::White, Strings::get("Ooze"), {}, {} });
	data.push_back({ L'p', L'p',  4, 3, 3, Color::White, Strings::get("Predator"), {}, {} });
	/* TODO: */ data.push_back({ L'q', L'q',  3, 1, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'r', L'r',  3, 2, 1, Color::White, Strings::get("Raider"), { Weapon::CombatKnife, Weapon::Pistol, Weapon::AssaultRifle }, { Armor::Helmet, Armor::Shield } });
	data.push_back({ L's', L's',  3, 3, 1, Color::White, Strings::get("SpaceMarine"), { Weapon::CombatKnife, Weapon::Pistol, Weapon::SniperRifle, Weapon::RocketLauncher }, { Armor::Helmet, Armor::Shield, Armor::Body } });
	data.push_back({ L't', L't',  3, 2, 1, Color::White, Strings::get("Turret"), { Weapon::AssaultRifle }, {} });
	/* BlackHole */ data.push_back({ L'u', L'u',  3, 1, 2, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'v', L'v',  3, 2, 1, Color::White, Strings::get("Viper"), { Weapon::CombatKnife, Weapon::LaserCutlass, Weapon::Pistol }, { Armor::Helmet } });
	/* BlackHole */ data.push_back({ L'w', L'w',  3, 2, 3, Color::White, Strings::get("Something"), {}, {} });
	/* BlackHole */ data.push_back({ L'x', L'x',  3, 3, 1, Color::White, Strings::get("Something"), {}, {} });
	data.push_back({ L'y', L'y',  3, 3, 3, Color::White, Strings::get("Yeti"), {}, {} });
	data.push_back({ L'z', L'z',  3, 2, 2, Color::White, Strings::get("Zombie"), {}, {} });

	return data;
}