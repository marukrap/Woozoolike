#include "World.hpp"
#include "Renderer.hpp"
#include "Config.hpp"
#include "Color.hpp"

// Levels
#include "Planet.hpp"
#include "Spaceship.hpp"
#include "Ground.hpp"

#include "Actor.hpp"
#include "Vehicle.hpp"

#include "Strings.hpp"

#include <cassert>

World::World(sf::View& view, Config& config)
	: view(view)
	, config(config)
	, galaxy(config.mapSize, rng)
	, level(&galaxy)
	, sidebar(config)
	, log(config)
{
	GameObject::setWorld(*this);
	Level::initializeActorTable();

	// initialize the galaxy
	galaxy.build();

	fov.setAlgorithm(Fov::Bob);
	fov.setMap(*level); // galaxy
	dijkstra.setMap(*level); // galaxy

	// create player actor
	player = level->addPlayerActor();

	log.print(Strings::get("Welcome"), sf::Color::White, { galaxy.getName() });
	log.print(Strings::get("Welcome2"));
	log.print(Strings::get("Welcome3"), Color::Yellow);

	//
	updateFov();
}

void World::handleAction(Action action)
{
	sf::Vector2i dir;

	switch (action)
	{
	case Action::MoveLeft:
		dir.x = -1;
		break;

	case Action::MoveRight:
		dir.x = +1;
		break;

	case Action::MoveUp:
		dir.y = -1;
		break;

	case Action::MoveDown:
		dir.y = +1;
		break;

	case Action::MoveLeftUp:
		dir.x = -1;
		dir.y = -1;
		break;

	case Action::MoveLeftDown:
		dir.x = -1;
		dir.y = +1;
		break;

	case Action::MoveRightUp:
		dir.x = +1;
		dir.y = -1;
		break;

	case Action::MoveRightDown:
		dir.x = +1;
		dir.y = +1;
		break;

	case Action::Wait:
		endPlayerTurn(); // rest once
		break;

	case Action::Interact:
		interact();
		break;

	case Action::PickUp:
		pickUp();
		break;

	case Action::DropWeapon:
		dropWeapon();
		break;

	case Action::EnterOrExit:
		if (player->getVehicle())
			exitVehicle();
		else
			enterVehicle(); // enter a vehicle or pick up an item
		break;

	// TODO: rework
	case Action::CancelFire: // leave a planet or go inside a ship
		if (player->getVehicle())
		{
			if (level->getLevelType() == LevelType::Planet || level->getLevelType() == LevelType::BlackHole)
				leavePlanet();
			else
				enterShip();
		}

		#ifdef _DEBUG
		else if (level->getLevelType() == LevelType::Planet || level->getLevelType() == LevelType::BlackHole)
			leavePlanet();
		#endif
		break;

	case Action::PreviousWeapon: // mouse wheel up
		player->equipment.selectPrevWeapon();
		dirty();
		break;

	case Action::NextWeapon: // mouse wheel down
		player->equipment.selectNextWeapon();
		dirty();
		break;
	}

	if (dir.x != 0 || dir.y != 0)
		moveOrAttack(dir);
}

void World::update(sf::Time dt)
{
	projectileQueue.update(dt);

	if (turn == Turn::Enemy)
	{
		// you must compute fov before process enemy turn
		updateFov();

		player->finishTurn();

		auto& entities = (*level)[Level::ActorLayer];

		// HACK: do not update dijkstra map when there is no enemy
		if (entities.size() > 1)
			updateDijkstra();

		for (auto& entity : entities)
			dynamic_cast<Actor*>(entity.get())->updateAI();

		// remove dead actors
		level->removeWrecks();

		turn = Turn::Player;
	}

	// sidebar.update(dt);
	log.update(dt);

	// SoundPlayer::get().removeStoppedSounds();
}

void World::draw(Renderer& renderer)
{
	if (!isDirty)
		return;

	isDirty = false;

	renderer.clear(Renderer::TextLayer);

	level->draw(renderer);
	// fov.draw(renderer);
	projectileQueue.draw(renderer);

	updateView(renderer);

	// HACK:
	sidebar.refresh(*player);
}

Actor& World::getPlayerActor()
{
	return *player;
}

Level& World::getLevel()
{
	return *level;
}

DijkstraMap& World::getDijkstraMap()
{
	return dijkstra;
}

Sidebar& World::getSidebar()
{
	return sidebar;
}

MessageLog& World::getLog()
{
	return log;
}

ProjectileQueue& World::getProjectileQueue()
{
	return projectileQueue;
}

Galaxy & World::getGalaxy()
{
	return galaxy;
}

sf::Color World::getClearColor() const
{
	if (level->getLevelType() == LevelType::BlackHole)
		return sf::Color(5, 5, 5);

	return sf::Color(19, 23, 28);
}

sf::Color World::getBackgroundColor() const
{
	sf::Color color = getClearColor();
	color.a = 230;

	return color;
}

void World::dirty()
{
	isDirty = true;
}

void World::toggleFov()
{
	displayFov = !displayFov;
	
	updateFov();
	dirty();
}

void World::changeFov()
{
	if (fov.getAlgorithm() == Fov::Adam)
	{
		fov.setAlgorithm(Fov::Bob);
		log.print(L"Debug: Fov - Bob");
	}

	else // Bob
	{
		log.print(L"Debug: Fov - Adam");
		fov.setAlgorithm(Fov::Adam);
	}

	updateFov();
	dirty();
}

bool World::canTakeTurn()
{
	return turn == Turn::Player && projectileQueue.isEmpty() && !player->isDestroyed();
}

/*
void World::addProjectile(Actor& actor, const sf::Vector2i& target)
{
	sf::Vector2i p1 = actor.getPosition();
	sf::Vector2i p2 = target;

	assert(actor.getPosition() != target);

	std::vector<sf::Vector2i> path = plotPath(actor.getPosition(), target);

	// UNDONE: fire!
	if (projectile)
		projectileQueue.emplace_back(std::make_unique<Projectile>(std::move(path), actor));
	else
		projectile = std::make_unique<Projectile>(std::move(path), actor);
}
*/

// TODO: avoid using sf::View (make Camera class for scrolling)
void World::updateView(Renderer& renderer)
{
	sf::Vector2i position = player->getPosition();
	sf::Vector2f tileSize = renderer.getTileSize();

	sf::Vector2f center;
	center.x = position.x * tileSize.x + tileSize.x / 2;
	center.y = position.y * tileSize.y + tileSize.y / 2;

	// view size, map size (in pixels)
	sf::Vector2f viewSize = view.getSize();
	sf::Vector2f mapSize(level->width * tileSize.x, level->height * tileSize.y);

	if (mapSize.x < viewSize.x)
		center.x = mapSize.x / 2.f;
	else if (center.x - viewSize.x / 2.f < 0.f)
		center.x = viewSize.x / 2.f;
	else if (center.x + viewSize.x / 2.f > mapSize.x)
		center.x = mapSize.x - viewSize.x / 2.f;

	if (mapSize.y < viewSize.y)
		center.y = mapSize.y / 2.f;
	else if (center.y - viewSize.y / 2.f < 0.f)
		center.y = viewSize.y / 2.f;
	else if (center.y + viewSize.y / 2.f > mapSize.y)
		center.y = mapSize.y - viewSize.y / 2.f;

	// TODO: smooth scrolling
	view.setCenter(std::floor(center.x), std::floor(center.y));
}

void World::updateFov()
{
	if (level->getLevelType() == LevelType::Galaxy || level->getLevelType() == LevelType::BlackHole)
		fov.compute(player->getPosition(), config.fov / 2);
	else // Planet, Spaceship
		fov.compute(player->getPosition(), config.fov);

	// turn off fov
	if (!displayFov)
	{
		for (int y = 0; y < level->height; ++y)
			for (int x = 0; x < level->width; ++x)
				level->at(x, y).visible = true;
	}
}

void World::updateDijkstra()
{
	dijkstra.reset();
	dijkstra.add(player->getPosition(), 0);
	dijkstra.update();
}

void World::endPlayerTurn()
{
	turn = Turn::Enemy;
	
	dirty();
}

void World::moveOrAttack(const sf::Vector2i& dir)
{
	sf::Vector2i position = player->getPosition() + dir;

	if (!level->isInBounds(position))
	{
		if (level == &galaxy || level->getLevelType() == LevelType::BlackHole)
		{
			if (position.x < 0)
				position.x = level->width - 1;
			else if (position.x > level->width - 1)
				position.x = 0;

			if (position.y < 0)
				position.y = level->height - 1;
			else if (position.y > level->height - 1)
				position.y = 0;
		}

		else if (level->getLevelType() == LevelType::Planet && player->getVehicle())
		{
			leavePlanet();
			return;
		}
	}

	Entity* vehicle = level->getEntity(position, Level::VehicleLayer);

	if (vehicle && player->getVehicle()) // blocked
		return;

	if (level->at(position).passable || player->getVehicle())
	{
		// REMOVE: dynamic_cast
		Actor* target = dynamic_cast<Actor*>(level->getEntity(position, Level::ActorLayer));

		if (target) // melee attack
			player->attack(*target, false);

		else // move
		{
			if (player->getVehicle() && player->getVehicle()->getFuel() <= 0)
			{
				log.print(Strings::get("FuelEmpty"), Color::Red);

				#ifdef _DEBUG
				log.print(L"The fuel is full now.", Color::Cyan);
				player->getVehicle()->changeFuel(player->getVehicle()->getMaxFuel());
				#endif

				return;
			}

			// player->move(dir);
			player->setPosition(position);

			if (level->getLevelType() == LevelType::BlackHole)
			{
				if (position == level->findTile(Location::WhiteHole))
					log.print(Strings::get("OnWhiteHole"));
				else if (position == level->findTile(Location::Singularity))
					log.print(Strings::get("OnSingularity"));
			}
		}

		endPlayerTurn();
	}
}

void World::interact()
{
	if (level->getLevelType() == LevelType::Spaceship)
	{
		// go outside of a ship
		if (player->getPosition() == level->findTile(Location::Hatch))
		{
			exitShip();
			exitVehicle();
		}

		// pilot a ship
		else if (player->getPosition() == level->findTile(Location::Cockpit))
		{
			#ifndef _DEBUG
			if ((*level)[Level::ActorLayer].size() > 1)
				log.print(Strings::get("PilotFail"), Color::Red);

			else
			#endif
			{
				exitShip();
				player->setVehicle(dynamic_cast<Vehicle*>(playerVehicle.get()));
			}
		}

		return;
	}

	#ifdef _DEBUG
	else
	#else
	else if (player->getVehicle())
	#endif
	{
		Entity* entity = level->getEntity(player->getPosition(), Level::PlanetLayer);

		// land on a planet
		if (entity)
		{
			// REMOVE: dynamic_cast
			landOnPlanet(dynamic_cast<Planet&>(*entity));
			return;
		}
	}

	if (level->getLevelType() == LevelType::BlackHole)
	{
		if (player->getPosition() == level->findTile(Location::WhiteHole))
		{
			player->victory = 1;
			return;
		}

		else if (player->getPosition() == level->findTile(Location::Singularity))
		{
			player->victory = 2;
			return;
		}
	}

	// rest once
	endPlayerTurn();
}

void World::pickUp()
{
	Entity* entity = level->getEntity(player->getPosition(), Level::ItemLayer);

	if (entity)
	{
		Item* item = dynamic_cast<Item*>(entity);

		if (item->apply(*player))
			level->detachEntity(*item, Level::ItemLayer);

		endPlayerTurn();
	}
}

void World::dropWeapon()
{
	// drop equipped weapon
	if (level != &galaxy && !player->getVehicle())
	{
		player->equipment.dropCurrentWeapon();

		endPlayerTurn();
	}
}

void World::enterVehicle()
{
	Entity* entity = level->getEntity(player->getPosition(), Level::VehicleLayer);

	if (entity)
	{
		// enter a vehicle
		player->setVehicle(dynamic_cast<Vehicle*>(entity));
		playerVehicle = level->detachEntity(*entity, Level::VehicleLayer);

		// log.print(L"You board in " + vehicle.getName(Article::The) + L".");

		if (!entity->getLevel() || !(*entity->getLevel())[Level::ActorLayer].empty())
			invadeSpaceship();

		endPlayerTurn();
	}

	else
		pickUp();
}

void World::exitVehicle()
{
	if (level->at(player->getPosition()).passable)
	{
		player->setVehicle(nullptr);
		level->attachEntity(std::move(playerVehicle), Level::VehicleLayer);
		// playerVehicle = nullptr;

		// log.print(L"You disembark from the ship.");

		endPlayerTurn();
	}
}

void World::landOnPlanet(Planet& planet)
{
	// land on a planet
	Level* inside = planet.getLevel();

	// generate a level
	if (!inside)
	{
		auto innerMap = std::make_unique<Ground>(config.mapSize, rng, planet.getInfo());
		innerMap->build();

		inside = innerMap.get();
		planet.attachLevel(std::move(innerMap));
	}

	if (inside->getLevelType() == LevelType::BlackHole)
	{
		// you must have 3 pieces of the amulet at least
		if (player->amulet < 3)
		{
			log.print(Strings::get("LandBlackHole"), Color::Red);
			#ifndef _DEBUG
			return;
			#endif
		}

		else // >= 3
			log.print(Strings::get("LandBlackHole2"), Color::Cyan);
	}

	else
		log.print(Strings::get("Landed"), Color::Yellow);

	changeLevel(*inside);

	// HACK: prevent overlap between actors
	while (true)
	{
		// TODO: landingPoint
		sf::Vector2i position = level->findTile(Location::Random);
		
		if (!level->getEntity(position, Level::ActorLayer))
		{
			player->setPosition(position);
			break;
		}
	}

	currentPlanet = &planet;

	// log.print(L"You land softly on the planet.", Color::Yellow);

	endPlayerTurn();
}

void World::leavePlanet()
{
	// back to the universe
	changeLevel(galaxy);
	player->setPosition(currentPlanet->getPosition());
	currentPlanet = nullptr;

	log.print(Strings::get("Left"), Color::Yellow);

	endPlayerTurn();
}

void World::invadeSpaceship()
{
	Level* inside = playerVehicle->getLevel();

	// generate a level
	if (!inside)
	{
		auto innerMap = std::make_unique<Spaceship>(config.mapSize / 2, rng);
		innerMap->build();

		inside = innerMap.get();
		playerVehicle->attachLevel(std::move(innerMap));
	}

	changeLevel(*inside);
	player->setVehicle(nullptr);

	// HACK: prevent overlap between actors
	sf::Vector2i position = level->findTile(Location::Hatch);
	Entity* actor = level->getEntity(position, Level::ActorLayer);
	
	if (actor)
		level->detachEntity(*actor, Level::ActorLayer);

	player->setPosition(position);

	log.print(Strings::get("Invade"), Color::Yellow);
}

void World::enterShip()
{
	if (level->getLevelType() == LevelType::Spaceship)
		return;

	changeLevel(*playerVehicle->getLevel());
	player->setVehicle(nullptr);
	player->setPosition(level->findTile(Location::Cockpit));

	log.print(Strings::get("Cockpit"), Color::Yellow);

	endPlayerTurn();
}

void World::exitShip()
{
	changeLevel(galaxy);
	player->setPosition(playerVehicle->getPosition());
}

void World::changeLevel(Level& level)
{
	level.attachEntity(this->level->detachEntity(*player, Level::ActorLayer), Level::ActorLayer);
	fov.setMap(level);
	dijkstra.setMap(level);
	this->level = &level;

	dirty();
}