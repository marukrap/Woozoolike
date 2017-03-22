#include "Ground.hpp"
#include "Rng.hpp"
#include "Color.hpp"
#include "Planet.hpp" // HACK: PlanetInfo

namespace
{
	// int wallprob = 40;
	int repeat1 = 4;
	int repeat2 = 3;
	int r1cutoff = 5;
	int r2cutoff = 2;
}

Ground::Ground(const sf::Vector2i& size, Rng& rng, const PlanetInfo& info)
	: Level(size, rng)
	, info(info)
{
}

LevelType Ground::getLevelType() const
{
	if (info.theme == Planet::BlackHole)
		return LevelType::BlackHole;

	return LevelType::Planet;
}

sf::Vector2i Ground::findTile(Location location) const
{
	if (location == Location::WhiteHole)
		return whiteHole;

	else if (location == Location::Singularity)
		return singularity;

	// else if (location == LandingPoint)
		// return landingPoint;

	return Level::findTile(location);
}

void Ground::onGenerate()
{
	// generate a simple cave-like level using cellular automata
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Tile& tile = at(x, y);

			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
				tile.glyph = Tile::Wall;
			else if (rng.getInt(100) < info.wallprob)
				tile.glyph = Tile::Wall;
			else
				tile.glyph = Tile::Floor;
		}

	for (int i = 0; i < repeat1; ++i)
		generation(r1cutoff, r2cutoff);

	for (int i = 0; i < repeat2; ++i)
		generation(r1cutoff);
}

void Ground::onDecorate()
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Tile& tile = at(x, y);

			if (tile.glyph == Tile::Floor)
			{
				// tile.glyph = Tile::Floor;
				// tile.tileNumber = info.tileNumber;
				tile.tileNumber = info.tileNumber + (rng.getInt(9) == 0 ? 1 : 0);
				tile.color = info.floorColor * sf::Color(100, 100, 100);
				tile.passable = true;
				tile.transparent = true;

				// HACK: apply the planet theme
				if (info.theme == Planet::Desert)
				{
					switch (rng.getInt(20))
					{
					case 0:
						tile.tileNumber = info.tileNumber + 1;
						break;

					case 1:
						// 0x1c, 0x1d: bones, cactus
						switch (rng.getInt(3))
						{
						case 0:
							tile.tileNumber = 0x1c;
							break;

						case 1:
							tile.glyph = Tile::Wall;
							tile.tileNumber = 0x1d;
							tile.color = Color::Green;
							tile.passable = false;
							break;

						case 2:
							// tile.tileNumber = info.tileNumber;
							break;
						}
						break;

					default:
						// tile.tileNumber = info.tileNumber;
						break;
					}
				}

				else if (info.theme == Planet::BlackHole)
					tile.color = info.wallColor;
			}

			else // Wall
			{
				// tile.glyph = Tile::Wall;
				tile.tileNumber = info.tileNumber + rng.getInt(2, 3);
				tile.color = info.wallColor;

				// HACK: apply the planet theme
				if (info.theme == Planet::Desert)
				{
					if (rng.getInt(2) == 0)
						tile.tileNumber = info.tileNumber + 2;
					else
						tile.tileNumber = info.tileNumber + 3;
				}

				else if (info.theme == Planet::Burning)
				{
					// TODO: if you step on a burning tile, you take damage

					if (rng.getInt(2) == 0)
						tile.tileNumber = info.tileNumber + 2;
					else
						tile.tileNumber = info.tileNumber + 3;
				}

				else if (info.theme == Planet::Ocean)
				{
					tile.glyph = L'~';
					tile.transparent = true;

					// 0x1e, 0x1f: lotus
					switch (rng.getInt(100))
					{
					case 0:
						tile.tileNumber = 0x1e;
						tile.color = Color::DarkGreen;
						break;

					case 1:
						tile.tileNumber = 0x1f;
						tile.color = Color::DarkGreen;
						break;
					}
				}

				else if (info.theme == Planet::BlackHole)
				{
					tile.glyph = Tile::Floor;
					tile.color = info.floorColor * sf::Color(100, 100, 100);
					tile.transparent = true;
				}
			}
		}

	if (info.theme == Planet::BlackHole)
	{
		while (whiteHole == singularity)
		{
			whiteHole = findTile(Location::Random);
			singularity = findTile(Location::Random);
		}

		at(whiteHole).glyph = L'*';
		at(whiteHole).tileNumber = L',';
		at(whiteHole).color = Color::White;

		at(singularity).glyph = L'*';
		at(singularity).tileNumber = L'*';
		at(singularity).color = Color::Cyan;
	}
}

void Ground::onInitialize()
{
	addItems(rng.getInt(5, 10));
	// addActors(rng.getInt(10, 20));

	addAmulet();

	std::wstring actorList;

	switch (info.theme)
	{
	case Planet::Grassy:
		// alien, birdian, mutant, e. mutant, zombie
		actorList = L"aabbmmez";
		break;

	case Planet::Desert:
		// birdian, clay, ilyth, raider, predator
		actorList = L"bbccciirrp";
		break;

	case Planet::Burning:
		// raider, space marine, viper, ilyth, turret
		actorList = L"rrssvvit";
		break;

	case Planet::Snowy:
		// clay, ooze, viper, yeti, zombie
		actorList = L"coovyyzz";
		break;

	case Planet::Ocean:
		// birdian, mutant, e. mutant, folyp, ooze, raider
		actorList = L"bmefor";
		break;

	case Planet::BlackHole:
		// something x3
		actorList = L"uwx";
		break;
	}

	addActors(rng.getInt(10, 20), actorList);
}

void Ground::generation(int r1cutoff, int r2cutoff)
{
	std::vector<int> grid(width * height, Tile::Wall);

	for (int y = 1; y < height - 1; ++y)
		for (int x = 1; x < width - 1; ++x)
		{
			int r1 = 0, r2 = 0;

			for (int dy = -1; dy <= 1; ++dy)
				for (int dx = -1; dx <= 1; ++dx)
				{
					if (at(x + dx, y + dy).glyph == Tile::Wall)
						r1 += 1;
				}

			for (int dy = -2; dy <= 2; ++dy)
				for (int dx = -2; dx <= 2; ++dx)
				{
					if (std::abs(dx) == 2 && std::abs(dy) == 2)
						continue;

					if (!isInBounds(x + dx, y + dy))
						continue;

					if (at(x + dx, y + dy).glyph == Tile::Wall)
						r2 += 1;
				}

			if (r1 >= r1cutoff || r2 <= r2cutoff)
				grid[x + y * width] = Tile::Wall;
			else
				grid[x + y * width] = Tile::Floor;
		}

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			at(x, y).glyph = grid[x + y * width];
}