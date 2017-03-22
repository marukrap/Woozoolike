#pragma once

#include "GameObject.hpp"

#include <string>

class Actor;
class Weapon;
class Armor;

class Combat : public GameObject
{
public:
	Combat(Actor& attacker, Actor& defender, bool ranged);

	void attack();

private:
	bool hit();
	void damage();

	bool avoid(Armor& armor);
	bool block(Armor& armor, int& damage);

	std::wstring makePossessive(const std::wstring& subject, const std::wstring& noun);

private:
	Actor& attacker;
	Actor& defender;
	bool ranged;
	int distance;

	Weapon* weapon = nullptr;
	Weapon* defenderWeapon = nullptr;
	
	Armor* shield = nullptr;
	Armor* helmet = nullptr;
	Armor* body = nullptr;
};