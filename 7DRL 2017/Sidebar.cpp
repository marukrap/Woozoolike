#include "Sidebar.hpp"
#include "Config.hpp"
#include "Actor.hpp"
#include "Vehicle.hpp"
#include "Utility.hpp" // capitalize

// MessageLog
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

Sidebar::Sidebar(Config& config)
	: Console(config.sidebarSize.x, config.sidebarSize.y, *config.font, config.fontSize)
{
	setPosition((config.screenSize.x - config.sidebarSize.x) * getTileSize().x, 0.f);
}

void Sidebar::refresh(Actor& actor)
{
	clear(); // Renderer::TextLayer
	setBox(0, 0, getSize().x, getSize().y, Color::White);

	int x = 2;
	int y = 1;

	Level& level = world->getLevel();

	printCenter(y++, level.getName(), sf::Color::White);

	drawLine(y++);

	int barSize = getSize().x - 2;

	// TODO: std::ceil
	setColor(x - 1, y, barSize * actor.getHp() / actor.getMaxHp(), 1, Color::Red);
	setString(x, y++, Strings::get("Health"), Color::White);

	Vehicle* vehicle = actor.getVehicle();

	if (vehicle)
	{
		setColor(x - 1, y, barSize * vehicle->getFuel() / vehicle->getMaxFuel(), 1, Color::DarkBrown);
		setString(x, y++, Strings::get("Fuel"), Color::White);
	}

	else
	{
		setColor(x - 1, y, barSize * actor.getO2() / actor.getMaxO2(), 1, Color::Blue);
		setString(x, y++, Strings::get("Oxygen"), Color::White);
	}

	// HACK: Equipment
	Armor* body = actor.equipment.getArmor(Armor::Body);
	Armor* helmet = actor.equipment.getArmor(Armor::Helmet);
	Armor* shield = actor.equipment.getArmor(Armor::Shield);

	if (body)
	{
		setColor(x - 1, y, barSize * body->getDefense() / body->getMaxDefense(), 1, Color::DarkGreen);
		setString(x, y++, Strings::get("Armor"), Color::White);
	}

	if (helmet)
	{
		setColor(x - 1, y, barSize * helmet->getDefense() / helmet->getMaxDefense(), 1, Color::Green);
		setString(x, y++, capitalize(Strings::get("Helmet")), Color::White);
	}

	if (shield)
	{
		setColor(x - 1, y, barSize * shield->getDefense() / shield->getMaxDefense(), 1, Color::DarkBlue);
		setString(x, y++, capitalize(Strings::get("Shield")), Color::White);
	}

	drawLine(y++);

	if (vehicle)
	{
		printCenter(y++, vehicle->getName(), Color::Brown);
		
		drawLine(y++);

		printCommand(x, y++, L"e", Strings::get("ExitShip"));

		// if (vehicle->getWeapon())
			// printCommand(x, y++, L"f", Strings::get("Fire"));

		if (level.getLevelType() == LevelType::Planet)
			printCommand(x, y++, L"Tab", Strings::get("TakeOff"));
		else // Galaxy
			printCommand(x, y++, L"Tab", Strings::get("GoInside"));

		drawLine(y++);
	}

	else
	{
		Weapon* weapon = actor.equipment.getCurrentWeapon();

		if (weapon && level.getLevelType() != LevelType::Galaxy)
		{
			std::wstring name = weapon->getName();

			// HACK: English
			if (Strings::getLanguage() == Language::English)
			{
				switch (weapon->getWeaponType())
				{
				case Weapon::AssaultRifle: name = L"a. rifle"; break;
				case Weapon::SniperRifle: name = L"s. rifle"; break;
				case Weapon::RocketLauncher: name = L"r. launcher"; break;
				}
			}
			//

			if (weapon->getWeaponType() >= Weapon::Pistol)
				name += L" (" + std::to_wstring(weapon->getAmmo()) + L"/" + std::to_wstring(weapon->getMaxAmmo()) + L")";

			printCenter(y++, name, Color::Brown);
			// drawLine(y++);

			printCommand(x, y++, L"q/w", Strings::get("WeaponSwap"));
			printCommand(x, y++, L"d", Strings::get("WeaponDrop"));
			drawLine(y++);
		}

		Entity* vehicle = level.getEntity(actor.getPosition(), Level::VehicleLayer);

		if (vehicle)
		{
			printCenter(y++, vehicle->getName(), sf::Color::White);
			drawLine(y++);

			printCommand(x, y++, L"e", Strings::get("EnterShip"));
			drawLine(y++);

			return;
		}

		Entity* item = level.getEntity(actor.getPosition(), Level::ItemLayer);

		if (item)
		{
			// TODO: print ammo if an item is a weapon

			printCenter(y++, item->getName(), sf::Color::White);
			// drawLine(y++);

			printCommand(x, y++, L"e", Strings::get("PickUp"));
			drawLine(y++);
		}

		if (level.getLevelType() == LevelType::Spaceship)
		{
			if (actor.getPosition() == level.findTile(Location::Hatch))
			{
				printCommand(x, y++, L"Space", Strings::get("GoOutside"));
				drawLine(y++);

				return;
			}

			else if (actor.getPosition() == level.findTile(Location::Cockpit))
			{
				printCommand(x, y++, L"Space", Strings::get("PilotShip"));
				drawLine(y++);

				return;
			}
		}

		else if (level.getLevelType() == LevelType::BlackHole)
		{
			if (actor.getPosition() == level.findTile(Location::WhiteHole))
			{
				printCommand(x, y++, L"Space", Strings::get("Escape"));
				drawLine(y++);
				
				return;
			}

			else if (actor.getPosition() == level.findTile(Location::Singularity))
			{
				printCommand(x, y++, L"Space", Strings::get("Destroy"));
				drawLine(y++);

				return;
			}
		}

		printCommand(x, y++, L"Space", Strings::get("Wait"));
		drawLine(y++);
	}

	Entity* planet = level.getEntity(actor.getPosition(), Level::PlanetLayer);

	if (planet)
	{
		printCenter(y++, planet->getName(), sf::Color::White);
		drawLine(y++);

		if (vehicle)
		{
			printCommand(x, y++, L"Space", Strings::get("LandOn"));
			drawLine(y++);
		}
	}
}

void Sidebar::drawLine(int y)
{
	setBox(1, y, getSize().x - 2, 1, Color::DarkGray);
}

void Sidebar::printCenter(int y, const std::wstring& string, const sf::Color& color)
{
	// setString(getSize().x / 2 - string.size() / 2, y, capitalize(string), color);
	setString(2, y, capitalize(string), color);
}

void Sidebar::printCommand(int x, int y, const std::wstring& command, const std::wstring& description)
{
	setString(x, y, command, Color::Yellow);
	setString(x + command.size(), y, L": " + description, Color::White);
}