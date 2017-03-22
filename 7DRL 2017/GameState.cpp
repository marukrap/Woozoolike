#include "GameState.hpp"
#include "Renderer.hpp"
#include "Actor.hpp"
#include "Utility.hpp" // plotPath

// MessageLog
#include "Strings.hpp"
#include "Color.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

GameState::GameState(StateStack& stack)
	: State(stack)
	, world(*getContext().view, *getContext().config)
	, actor(world.getPlayerActor())
{
	getContext().world = &world;
}

GameState::~GameState()
{
	getContext().world = nullptr;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return true;

	Action action = player.getAction(event.key.code);

	switch (event.key.code)
	{
	case sf::Keyboard::Escape:
		if (!path.empty()) // you're choosing target
			handleAction(action);
		else
			requestStackPush("Pause");
		break;

	case sf::Keyboard::F1:
		requestStackPush("Help");
		break;

	case sf::Keyboard::Slash:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			requestStackPush("Help");
		break;

	#ifdef _DEBUG
	case sf::Keyboard::F5:
		world.toggleFov();
		break;

	case sf::Keyboard::F6:
		world.changeFov();
		break;
	#endif

	case sf::Keyboard::M:
		requestStackPush("MessageLog");
		break;
	}

	if (!world.canTakeTurn())
		return true;

	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			// do nothing
			break;

		case sf::Keyboard::F:
			if (path.empty())
				chooseTarget();
			else
				fire();
			break;

		case sf::Keyboard::Tilde: // bare hands
			actor.equipment.selectWeapon(-1);
			world.dirty();
			break;

		// case sf::Keyboard::Num0: // bare hands
		case sf::Keyboard::Num1: // combat knife
		case sf::Keyboard::Num2: // laser cutlass
		case sf::Keyboard::Num3: // pistol
		case sf::Keyboard::Num4: // shotgun
		case sf::Keyboard::Num5: // assault rifle
		case sf::Keyboard::Num6: // sniper rifle
		case sf::Keyboard::Num7: // rocket launcher
			actor.equipment.selectWeapon(event.key.code - sf::Keyboard::Num0 - 1);
			world.dirty();
			break;

		default:
			if (path.empty())
				world.handleAction(action);
			else
				handleAction(action);
			break;
		}
	}

	return true;
}

bool GameState::update(sf::Time dt)
{
	Renderer& renderer = *getContext().renderer;

	world.update(dt);
	world.draw(renderer);

	if (actor.victory > 0) // world.victory
		requestStackPush("Victory");

	if (dirty)
	{
		dirty = false;
		renderer.clear(Renderer::Foreground);

		for (std::size_t i = 0; i < path.size(); ++i)
		{
			sf::Color color = sf::Color(0, 255, 0, 25);
			Entity* entity = world.getLevel().getEntity(path[i], Level::ActorLayer);

			if (entity)
				color = sf::Color(255, 0, 0, 50);
			else if (i == path.size() - 1)
				color = sf::Color(0, 255, 0, 50);

			renderer.setColor(path[i].x, path[i].y, color, Renderer::Foreground);
		}
	}

	return true;
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	sf::View& view = *getContext().view;
	Renderer& renderer = *getContext().renderer;

	window.setView(view);
	window.draw(renderer);

	window.setView(window.getDefaultView());
	window.draw(world.getLog());
	window.draw(world.getSidebar());
}

void GameState::handleAction(Action action)
{
	sf::Vector2i dir;

	switch (action)
	{
	case Action::MoveLeft:
		dir.x = -1;
		break;

	case Action::MoveRight:
		dir.x = 1;
		break;

	case Action::MoveUp:
		dir.y = -1;
		break;

	case Action::MoveDown:
		dir.y = 1;
		break;

	case Action::MoveLeftUp:
		dir.x = -1;
		dir.y = -1;
		break;

	case Action::MoveLeftDown:
		dir.x = -1;
		dir.y = 1;
		break;

	case Action::MoveRightUp:
		dir.x = 1;
		dir.y = -1;
		break;

	case Action::MoveRightDown:
		dir.x = 1;
		dir.y = 1;
		break;

	case Action::PreviousWeapon: // PreviousTarget
		// if (!entities.empty())
		// {
			currentEntity = (currentEntity + entities.size() - 1) % entities.size();

			if (currentEntity == 0 && entities.size() > 1)
				currentEntity = entities.size() - 1;

			drawPath(entities[currentEntity]->getPosition());
		// }
		break;

	case Action::NextWeapon: // NextTarget
		// if (!entities.empty())
		// {
			currentEntity = (currentEntity + 1) % entities.size();

			if (currentEntity == 0 && entities.size() > 1)
				currentEntity = 1;

			drawPath(entities[currentEntity]->getPosition());
		// }
		break;

	case Action::CancelFire:
		path.clear();
		dirty = true;
		break;
	}

	if (dir.x != 0 || dir.y != 0)
	{
		sf::Vector2i position = cursor + dir;
		int distance = length(position - actor.getPosition());
		Weapon* weapon = actor.equipment.getCurrentWeapon();

		if (world.getLevel().isInBounds(position) && distance <= weapon->getRange()) // && world.getLevel().at(position).visible)
			drawPath(position);
	}
}

void GameState::drawPath(const sf::Vector2i& position)
{
	// path.clear();
	dirty = true;
	path = plotPath(actor.getPosition(), position);
	cursor = position;
}

void GameState::chooseTarget()
{
	Vehicle* vehicle = actor.getVehicle();
	Weapon* weapon = actor.equipment.getCurrentWeapon();

	if (vehicle) // && world.getLevel().getLevelType() != LevelType::Spaceship)
	{
		// weapon = vehicle->getWeapon();

		// if (weapon && distance <= weapon->getRange())
			// TODO: launch missile
	}
	
	else if (weapon && weapon->getRange() > 1)
	{
		if (weapon->getAmmo() <= 0)
		{
			world.getLog().print(Strings::get("OutOfAmmo"), Color::Red);
			return;
		}

		entities = world.getLevel().getVisibleEntities(Level::ActorLayer);

		// sort the entities by distance
		std::sort(entities.begin(), entities.end(), [this] (const Entity* e1, const Entity* e2)
		{
			int d1 = length(e1->getPosition() - actor.getPosition());
			int d2 = length(e2->getPosition() - actor.getPosition());

			return d1 < d2;
		});
		
		// REMOVE: the distances of entities are calculated twice
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			int distance = length((*it)->getPosition() - actor.getPosition());

			if (distance > weapon->getRange())
			{
				entities.erase(it, entities.end());
				break;
			}
		}

		currentEntity = entities.size() > 1 ? 1 : 0;

		// choose the previous target first
		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			if (entities[i] == actor.getLastTarget())
			{
				currentEntity = i;
				break;
			}
		}

		drawPath(entities[currentEntity]->getPosition());
	}
}

void GameState::fire()
{
	if (actor.getPosition() == path.front())
		return;

	int bullets = 1;

	Weapon* weapon = actor.equipment.getCurrentWeapon();

	if (actor.getVehicle())
	{
		// TODO: launch missile
	}

	else if (weapon->getWeaponType() == Weapon::AssaultRifle)
		bullets = weapon->getAmmo() >= 3 ? 3 : weapon->getAmmo();

	world.getProjectileQueue().add(actor, std::move(path), bullets);
	weapon->changeAmmo(-bullets);

	dirty = true;
}