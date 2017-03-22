#pragma once

class World;

class GameObject
{
public:
	static void setWorld(World& world);

protected:
	static World* world;
};