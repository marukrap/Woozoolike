#pragma once

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

class GameState : public State
{
public:
	explicit GameState(StateStack& stack);
	~GameState();

	bool handleEvent(const sf::Event& event) override;
	bool update(sf::Time dt) override;
	void draw() override;

private:
	void handleAction(Action action);
	void drawPath(const sf::Vector2i& position);

	void chooseTarget();
	void fire();

private:
	World world;
	Player player;
	Actor& actor;
	bool dirty = true;

	// target interface
	sf::Vector2i cursor;
	std::vector<sf::Vector2i> path;
	int currentEntity = -1;
	std::vector<Entity*> entities;
};