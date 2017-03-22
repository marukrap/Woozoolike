#include "Spaceship.hpp"
#include "Rng.hpp"
#include "Color.hpp"
#include "Direction.hpp"

#include <algorithm> // remove_if

int Spaceship::Room::right() const
{
	return x + width;
}

int Spaceship::Room::bottom() const
{
	return y + height;
}

LevelType Spaceship::getLevelType() const
{
	return LevelType::Spaceship;
}

sf::Vector2i Spaceship::findTile(Location location) const
{
	if (location == Location::Hatch)
		return hatch;

	else if (location == Location::Cockpit)
		return cockpit;

	return Level::findTile(location);
}

void Spaceship::onGenerate()
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			at(x, y).glyph = Tile::Wall;

	// create the first room
	Room room;
	room.width = 3;
	room.height = 3;
	room.x = rng.getInt(1, (width - room.width) / 2) * 2 - 1;
	room.y = rng.getInt(1, (height - room.height) / 2) * 2 - 1;

	carve(room);
	rooms.emplace_back(room);
	
	addRooms(1000);

	if (rooms.size() < 2)
	{
		rooms.clear();
		onGenerate();
	}
}

void Spaceship::onDecorate()
{
	carveCorners(rooms.back()); // cockpit

	removeWalls();
	colorize();
	trimWalls();

	decorateHatch(rooms.front()); // rooms[0]
	decorateCockpit(rooms.back());

	for (std::size_t i = 1; i < rooms.size() - 1; ++i)
		decorate(rooms[i]);
}

void Spaceship::onInitialize()
{
	addItems(rng.getInt(3, 7));

	// alien, birdian, mutant, e. mutant, police, ooze, predator, raider, viper
	addActors(rng.getInt(5, 10), L"aabbmegoprv");

	if (rng.getInt(3) == 0)
		addAmulet();

	// TODO: master Key (used for piloting the ship)
}

void Spaceship::colorize()
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Tile& tile = at(x, y);

			if (tile.glyph == Tile::Unused)
			{
				tile.glyph = Tile::Floor;
				tile.color = Color::Black; // Color::DarkGray
				tile.passable = false;
				tile.transparent = true;

				// decorate the outer space with stars
				int r = rng.getInt(100);

				if (r < 65)
					tile.tileNumber = 0;
				else if (r < 85) // 20
					tile.tileNumber = 1;
				else if (r < 95) // 10
					tile.tileNumber = 2;
				else // 5
					tile.tileNumber = 3;
			}

			else if (tile.glyph == Tile::Floor)
			{
				tile.tileNumber = L'.';
				tile.color = Color::DarkGray;
				tile.passable = true;
				tile.transparent = true;
			}

			else if (tile.glyph == Tile::Wall)
			{
				tile.tileNumber = L'#';
				tile.color = Color::LightGray;
				tile.passable = false;
				tile.transparent = false;
			}

			else if (tile.glyph == Tile::ClosedDoor)
			{
				tile.tileNumber = L'+';
				tile.passable = true;
				tile.transparent = false;
			}

			else if (tile.glyph == Tile::OpenDoor)
			{
				tile.tileNumber = L'-';
				tile.passable = true;
				tile.transparent = true;
			}
		}
}

void Spaceship::addRooms(int tries)
{
	for (int i = 0; i < tries; ++i)
	{
		Room room;
		// room.width = rng.rollDice(3, 5); // 3 - 15
		// room.height = rng.rollDice(3, 3); // 3 - 9
		// room.x = rng.getInt(1, width - room.width - 1);
		// room.y = rng.getInt(1, height - room.height - 1);

		// room.width = rng.getInt(1, 6) * 2 + 1;
		// room.height = rng.getInt(1, 4) * 2 + 1;
		room.width = rng.rollDice(3, 4) / 2 * 2 + 1;
		room.height = rng.rollDice(3, 3) / 2 * 2 + 1;
		room.x = rng.getInt(1, (width - room.width) / 2) * 2 - 1;
		room.y = rng.getInt(1, (height - room.height) / 2) * 2 - 1;

		if (canCarve(room))
			connect(room);
	}
}

bool Spaceship::canCarve(const Room& room) const
{
	for (int y = room.y - 1; y < room.bottom() + 1; ++y)
		for (int x = room.x - 1; x < room.right() + 1; ++x)
		{
			if (at(x, y).glyph != Tile::Wall)
				return false;
		}

	return true;
}

void Spaceship::carve(const Room& room)
{
	for (int y = room.y; y < room.bottom(); ++y)
		for (int x = room.x; x < room.right(); ++x)
			at(x, y).glyph = Tile::Floor;
}

bool Spaceship::isCarved(int x, int y) const
{
	if (!isInBounds(x, y))
		return false;

	return at(x, y).glyph == Tile::Floor;
}

void Spaceship::connect(const Room& room)
{
	std::vector<sf::Vector2i> connectors;

	for (int y = room.y; y < room.bottom(); ++y)
	{
		if (isCarved(room.x - 2, y))
			connectors.emplace_back(room.x - 1, y);

		if (isCarved(room.right() + 1, y))
			connectors.emplace_back(room.right(), y);
	}

	for (int x = room.x; x < room.right(); ++x)
	{
		if (isCarved(x, room.y - 2))
			connectors.emplace_back(x, room.y - 1);

		if (isCarved(x, room.bottom() + 1))
			connectors.emplace_back(x, room.bottom());
	}

	if (!connectors.empty())
	{
		auto connector = connectors[rng.getInt(connectors.size())];
		at(connector).glyph = Tile::ClosedDoor;

		// erase-remove
		auto it = std::remove_if(connectors.begin(), connectors.end(), [&] (const auto& c)
		{
			return c.x == connector.x || c.y == connector.y;
		});
		connectors.erase(it, connectors.end());

		if (!connectors.empty() && rng.getInt(2) == 0)
		{
			auto another = connectors[rng.getInt(connectors.size())];
			at(another).glyph = Tile::OpenDoor;
		}

		carve(room);
		rooms.emplace_back(room);
	}
}

void Spaceship::decorate(Room& room)
{
	decorateFloor(room);
	placeObjects(room);

	// decorate walls
	// decorateWindows(room);
}

bool Spaceship::decorateFloor(Room& room)
{
	switch (rng.getInt(4))
	{
	case 0:
		// #######
		// #.....#
		// #.....#
		// #.....#
		// #.....#
		// #.....#
		// #######
		return false;

	case 1:
		// #######
		// #.....#
		// #.xxx.#
		// #.xxx.#
		// #.xxx.#
		// #.....#
		// #######
		for (int y = room.y + 1; y < room.bottom() - 1; ++y)
			for (int x = room.x + 1; x < room.right() - 1; ++x)
				at(x, y).tileNumber = 0xdb;
		return true;

	case 2:
		// #######
		// #x...x#
		// #.....#
		// #.....#
		// #.....#
		// #x...x#
		// #######
		at(room.x, room.y).tileNumber = 0xb1;
		at(room.right() - 1, room.y).tileNumber = 0xb1;
		at(room.x, room.bottom() - 1).tileNumber = 0xb1;
		at(room.right() - 1, room.bottom() - 1).tileNumber = 0xb1;
		return true;

	case 3:
		// #######
		// #.....#
		// #.x.x.#
		// #.....#
		// #.x.x.#
		// #.....#
		// #######
		at(room.x + 1, room.y + 1).tileNumber = 0xb1;
		at(room.right() - 2, room.y + 1).tileNumber = 0xb1;
		at(room.x + 1, room.bottom() - 2).tileNumber = 0xb1;
		at(room.right() - 2, room.bottom() - 2).tileNumber = 0xb1;
		return true;
	}

	return false;
}

void Spaceship::decorateHatch(Room& room)
{
	for (int y = room.y; y < room.bottom(); ++y)
		for (int x = room.x; x < room.right(); ++x)
		{
			Tile& tile = at(x, y);

			if (tile.glyph == Tile::Floor)
			{
				tile.tileNumber = 0xb0;
				tile.color = Color::Yellow;
			}
		}

	hatch.x = room.x + room.width / 2;
	hatch.y = room.y + room.height / 2;

	Tile& tile = at(hatch);

	tile.glyph = Tile::DownStairs;
	tile.tileNumber = L'>';
	tile.color = Color::Cyan;
}

void Spaceship::decorateCockpit(Room& room)
{
	for (int y = room.y; y < room.bottom(); ++y)
		for (int x = room.x; x < room.right(); ++x)
		{
			Tile& tile = at(x, y);

			if (tile.glyph == Tile::Floor)
			{
				tile.tileNumber = 0xb2;
				tile.color = Color::Blue;
			}
		}

	cockpit.x = room.x + room.width / 2;
	cockpit.y = room.y + room.height / 2;

	Tile& tile = at(cockpit);

	tile.glyph = Tile::UpStairs;
	tile.tileNumber = L'<';
	tile.color = Color::Cyan;
}

void Spaceship::placeObjects(Room& room)
{
	bool placed = false;

	if (counters[Storeroom] < 2 && rng.getInt(3) == 0)
	{
		for (int y = room.y + 1; y < room.bottom() - 1; y += 2)
		{
			if (room.width % 4 == 1)
			{
				for (int x = room.x + 1; x < room.right() - 3; x += 4)
					placed |= placeObstacles(x, y);
			}

			else
			{
				if (room.width > 9)
				{
					placed |= placeObstacles(room.x + 1, y);
					placed |= placeObstacles(room.right() - 4, y);
				}

				else if (room.width > 4) // 5, 7
					placed |= placeObstacles(room.x + room.width / 2 - 1, y);
			}
		}

		if (placed)
			++counters[Storeroom];

		if (rng.getInt(2) == 0)
			decorateWindows(room);
	}

	else if (counters[Bedroom] < 2 && rng.getInt(3) == 0)
	{
		if (room.height % 3 == 1)
		{
			for (int y = room.y + 1; y < room.bottom() - 2; y += 3)
				for (int x = room.x + 1; x < room.right() - 1; x += 2)
				{
					if (rng.getInt(9) > 0)
						placed |= placeBed(x, y);
				}
		}

		else if (std::abs(room.width - room.height) < 3)
		{
			if (room.width > 2)
			{
				placed |= placeBed(room.x, room.y);
				placed |= placeBed(room.right() - 1, room.y);
			}

			if (room.height > 4)
			{
				placed |= placeBed(room.x, room.bottom() - 2);
				placed |= placeBed(room.right() - 1, room.bottom() - 2);
			}
		}

		if (placed)
			++counters[Bedroom];

		decorateWindows(room);
	}

	if (!placed)
	{
		// TODO: more special rooms

		if (counters[Library] < 2 && rng.getInt(3) == 0)
		{
			for (int y = room.y; y < room.bottom(); ++y)
				for (int x = room.x; x < room.right(); ++x)
				{
					if (x == room.x || x == room.right() - 1 || y == room.y || y == room.bottom() - 1)
					{
						// bookshelf: 0x117, 0x118, 0x119, 0x11a
						if (rng.getInt(9) > 0)
							placed |= placeObject(x, y, 0x25a0, 0x117 + rng.getInt(4), false, false, Color::Gray);
					}
				}

			if (placed)
				++counters[Library];
		}

		else if (counters[SleepPod] < 2 && rng.getInt(3) == 0)
		{
			for (int y = room.y + 1; y < room.bottom() - 2; y += 3)
				for (int x = room.x + 1; x < room.right() - 1; x += 2)
				{
					if (rng.getInt(9) > 0)
					{
						// sleep pod: 0x100, 0x101, 0x102, 0x103
						switch (rng.getInt(3))
						{
						case 0:
							placed |= placeObject(x, y, 0x2584, 0x100, false, true, Color::LightGray);
							break;

						case 1:
							placed |= placeObject(x, y, 0x2584, 0x101, false, true, Color::Gray);
							break;

						case 2:
							placed |= placeObject(x, y, 0x2584, 0x102, false, true, Color::Red);
							break;
						}

						placed |= placeObject(x, y + 1, 0x2580, 0x103, false, true, Color::LightGray);
					}
				}

			if (placed)
				++counters[SleepPod];
		}

		else if (rng.getInt(2) == 0)
			decorateWindows(room);
	}
}

bool Spaceship::blocksDoor(int x, int y) const
{
	const std::wstring doorTiles = { Tile::ClosedDoor, Tile::OpenDoor };

	for (auto dir : Direction::All) // Direction::Cardinal
	{
		if (doorTiles.find(at(x + dir.x, y + dir.y).glyph) != std::wstring::npos)
			return true;
	}

	return false;
}

bool Spaceship::placeObject(int x, int y, wchar_t glyph, int tileNumber, bool passable, bool transparent, const sf::Color& color)
{
	if (blocksDoor(x, y))
		return false;

	Tile& tile = at(x, y);

	tile.glyph = glyph; // Tile::Object;
	tile.tileNumber = tileNumber;
	tile.color = color;
	tile.passable = passable;
	tile.transparent = transparent;

	return true;
}

bool Spaceship::placeObstacles(int x, int y)
{
	bool placed = false;

	switch (rng.getInt(3))
	{
	case 0: // long table: 0x114, 0x115, 0x116 // 0x25a0: ■
		if (rng.getInt(9) > 0)
		{
			// TODO: place chairs
			placed |= placeObject(x, y, 0x25a0, 0x114, false, true, Color::LightGray);
			placed |= placeObject(x + 1, y, 0x25a0, 0x115, false, true, Color::LightGray);
			placed |= placeObject(x + 2, y, 0x25a0, 0x116, false, true, Color::LightGray);
		}
		break;

	case 1: // glass cases: 0x11b, 0x11c
		for (int i = x; i < x + 3; ++i)
		{
			if (rng.getInt(9) > 0)
				placed |= placeObject(i, y, 0x25a0, 0x11b + rng.getInt(2), false, true, Color::Green);
		}
		break;

	case 2: // boxes: 0x11d, 0x11e, 0x11f
		for (int i = x; i < x + 3; ++i)
		{
			if (rng.getInt(9) > 0)
				placed |= placeObject(i, y, 0x25a0, 0x11d + rng.getInt(3), false, false, Color::DarkBrown);
		}
		break;
	}

	return placed;
}

bool Spaceship::placeBed(int x, int y)
{
	if (blocksDoor(x, y) || blocksDoor(x, y + 1))
		return false;

	Tile& upper = at(x, y);
	Tile& lower = at(x, y + 1);

	upper.glyph = 0x2584; // ▄
	upper.tileNumber = 0x110 + rng.getInt(2) * 2;
	upper.color = sf::Color::White;
	upper.passable = false;
	upper.transparent = true;

	lower.glyph = 0x2580; // ▀
	lower.tileNumber = upper.tileNumber + 1;
	lower.color = Color::DarkGreen;
	lower.passable = false;
	lower.transparent = true;

	return true;
}

bool Spaceship::decorateWindows(Room& room)
{
	bool decorated = false;

	if (room.width % 2 == 1)
	{
		for (int x = room.x + 1; x < room.right() - 1; ++x)
			decorated |= placeWindow(x++, room.y - 1, { 0, -1 }, L'─', 0xd8);
			
		for (int x = room.x + 1; x < room.right() - 1; ++x)
			decorated |= placeWindow(x++, room.bottom(), { 0, +1 }, L'─', 0xd8);
	}

	if (room.height % 2 == 1)
	{
		for (int y = room.y + 1; y < room.bottom() - 1; ++y)
			decorated |= placeWindow(room.x - 1, y++, { -1, 0 }, L'│', 0xd7);
			
		for (int y = room.y + 1; y < room.bottom() - 1; ++y)
			decorated |= placeWindow(room.right(), y++, { +1, 0 }, L'│', 0xd7);
	}

	return decorated;
}

bool Spaceship::placeWindow(int x, int y, const sf::Vector2i& dir, wchar_t glyph, int tileNumber)
{
	if (!isInBounds(x + dir.x, y + dir.y) || at(x + dir.x, y + dir.y).glyph == Tile::Unused)
	{
		Tile& tile = at(x, y);

		tile.glyph = glyph;
		tile.tileNumber = tileNumber;
		tile.transparent = true;

		return true;
	}

	return false;
}

void Spaceship::carveCorners(const Room& room)
{
	carveCorner(room, room.x, room.y, { -1, -1 });
	carveCorner(room, room.x, room.bottom() - 1, { -1, 1 });
	carveCorner(room, room.right() - 1, room.y, { 1, -1 });
	carveCorner(room, room.right() - 1, room.bottom() - 1, { 1, 1 });
}

void Spaceship::carveCorner(const Room& room, int x, int y, const sf::Vector2i& dir)
{
	if (at(x + dir.x, y).glyph == Tile::Wall && at(x, y + dir.y).glyph == Tile::Wall)
	{
		at(x, y).glyph = Tile::Wall;

		if (room.width >= 5 && at(x - dir.x, y + dir.y).glyph == Tile::Wall)
			at(x - dir.x, y).glyph = Tile::Wall;

		if (room.height >= 5 && at(x + dir.x, y - dir.y).glyph == Tile::Wall)
			at(x, y - dir.y).glyph = Tile::Wall;
	}
}