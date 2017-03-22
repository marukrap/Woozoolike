#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <unordered_map>

enum class Action
{
	Unknown,
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	MoveLeftUp,
	MoveLeftDown,
	MoveRightUp,
	MoveRightDown,
	Interact, // interact with a tile
	PickUp,
	Wait, // rest once
	Fire,
	CancelFire,
	NextWeapon,
	PreviousWeapon,
	DropWeapon,
	EnterOrExit, // enter or exit a vehicle
};

class Player
{
public:
	Player();

	Action getAction(sf::Keyboard::Key key);

private:
	std::unordered_map<sf::Keyboard::Key, Action> keyBinding;
};