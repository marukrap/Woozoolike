#pragma once

#include "Rng.hpp"
#include "Galaxy.hpp"
#include "Fov.hpp"
#include "DijkstraMap.hpp"
#include "Player.hpp" // Action

// UI
#include "Sidebar.hpp"
#include "MessageLog.hpp"

// Effects
// #include "Particle.hpp"
#include "Projectile.hpp"

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Time.hpp>

struct Config;
class Renderer;

class Item;
class Vehicle;
class Planet;

enum class Turn
{
	Player,
	Enemy,
};

class World
{
public:
	World(sf::View& view, Config& config);

	void handleAction(Action action);
	void update(sf::Time dt);
	void draw(Renderer& renderer);

	Actor& getPlayerActor();
	Level& getLevel();
	DijkstraMap& getDijkstraMap();
	Sidebar& getSidebar();
	MessageLog& getLog();
	ProjectileQueue& getProjectileQueue();

	// UNDONE:
	Galaxy& getGalaxy();
	sf::Color getClearColor() const;
	sf::Color getBackgroundColor() const;
	//

	void dirty();
	void toggleFov();
	void changeFov(); // change fov algorithm

	bool canTakeTurn();
	void endPlayerTurn();

private:
	void updateView(Renderer& renderer);

	void updateFov();
	void updateDijkstra();
	// void endPlayerTurn();

	// player actions
	void moveOrAttack(const sf::Vector2i& dir);
	void interact();

	void pickUp();
	void dropWeapon();

	void enterVehicle();
	void exitVehicle();

	void landOnPlanet(Planet& planet);
	void leavePlanet();

	void invadeSpaceship();
	void enterShip();
	void exitShip();

	void changeLevel(Level& level);
	//

private:
	sf::View& view;
	Config& config;

	bool isDirty = true; // redraw scene
	bool displayFov = true;
	Turn turn = Turn::Player;
	
	Rng rng;
	Galaxy galaxy;
	Level* level = nullptr; // current level
	Fov fov;
	DijkstraMap dijkstra;

	Actor* player = nullptr;
	std::unique_ptr<Entity> playerVehicle = nullptr;
	Planet* currentPlanet = nullptr;

	// UI
	Sidebar sidebar;
	MessageLog log;
	
	ProjectileQueue projectileQueue;
};