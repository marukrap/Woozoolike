#include "GameObject.hpp"

World* GameObject::world = nullptr;

void GameObject::setWorld(World& world)
{
	GameObject::world = &world;
}