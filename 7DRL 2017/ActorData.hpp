#pragma once

#include "Weapon.hpp"
#include "Armor.hpp"

#include <vector>

// TODO: Human, Mutant, Machine(Robot), ...
struct SpeciesData
{
};

struct ActorData
{
	wchar_t glyph;
	int tileNumber;
	// TODO: hp+hd -> std::string hd;
	int hp;
	int hd;
	int damage;
	sf::Color color;
	std::wstring name;
	std::vector<Weapon::Type> weapons;
	std::vector<Armor::Type> armors;
};

std::vector<ActorData> initializeActorData();