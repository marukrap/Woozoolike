#pragma once

#include "GameObject.hpp"

class Actor;

class AI : public GameObject
{
public:
	explicit AI(Actor& actor);

	void update();

private:
	Actor& actor;
	int morale = 0; // anger
};