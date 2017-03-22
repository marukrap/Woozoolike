#include "Level.hpp"
#include "Rng.hpp"
#include "Utility.hpp" // lengthSquared
#include "Direction.hpp"

// entities
// #include "Actor.hpp"
// #include "Item.hpp"
#include "Vehicle.hpp"
#include "Planet.hpp"

// actors
#include "ActorPlayer.hpp"
#include "ActorEnemy.hpp"
#include "ActorData.hpp"

// items
#include "Weapon.hpp"
#include "Armor.hpp"
#include "Medkit.hpp"
#include "Amulet.hpp"

#include <algorithm> // find_if, remove_if
#include <functional> // mem_fn
#include <cassert>

namespace
{
	std::vector<ActorData> actorTable;
}

Level::Level(const sf::Vector2i& size, Rng& rng)
	: Map(size.x, size.y)
	, rng(rng)
{
}

void Level::build()
{
	onGenerate();
	onDecorate();
	onInitialize();
}

sf::Vector2i Level::findTile(Location location) const
{
	assert(location == Location::Random);

	while (true)
	{
		int x = rng.getInt(width);
		int y = rng.getInt(height);

		if (at(x, y).passable)
			return sf::Vector2i(x, y);
	}
}

void Level::setName(const std::wstring& name)
{
	this->name = name;
}

const std::wstring& Level::getName() const
{
	return name;
}

std::vector<Entity::Ptr>& Level::operator[](Layer layer)
{
	return layers[layer];
}

const std::vector<Entity::Ptr>& Level::operator[](Layer layer) const
{
	return layers[layer];
}

Entity* Level::getEntity(const sf::Vector2i& position, Layer layer)
{
	for (auto& entity : layers[layer])
	{
		if (entity->getPosition() == position)
			return entity.get();
	}

	return nullptr;
}

Entity* Level::getClosestEntity(const sf::Vector2i& position, Layer layer)
{
	Entity* closestEntity = nullptr;
	int closestDistance = INT_MAX;

	for (auto& entity : layers[layer])
	{
		int distance = lengthSquared(entity->getPosition() - position);

		if (distance != 0 && distance < closestDistance)
		{
			closestEntity = entity.get();
			closestDistance = distance;
		}
	}

	return closestEntity;
}

std::vector<Entity*> Level::getVisibleEntities(Layer layer)
{
	std::vector<Entity*> entities;

	for (auto& entity : layers[layer])
	{
		if (at(entity->getPosition()).visible)
			entities.emplace_back(entity.get());
	}

	return entities;
}

void Level::attachEntity(Entity::Ptr entity, Layer layer)
{
	layers[layer].emplace_back(std::move(entity));
}

Entity::Ptr Level::detachEntity(Entity& entity, Layer layer)
{
	EntityLayer& entities = layers[layer];

	auto found = std::find_if(entities.begin(), entities.end(), [&] (const auto& e) { return e.get() == &entity; });
	auto result = std::move(*found);
	entities.erase(found);

	return result;
}

void Level::removeWrecks(Layer layer)
{
	EntityLayer& entities = layers[layer];

	auto it = std::remove_if(entities.begin(), entities.end(), std::mem_fn(&Entity::isMarkedForRemoval));
	entities.erase(it, entities.end());
}

void Level::draw(Renderer& renderer)
{
	Map::draw(renderer);

	for (std::size_t layer = 0; layer < LayerCount; ++layer)
		for (const auto& entity : layers[layer])
		{
			Tile& tile = at(entity->getPosition());

			if (!tile.visible)
			{
				if (!tile.explored || layer == ActorLayer || layer == ItemLayer)
					continue;
			}

			entity->draw(renderer);
		}
}

void Level::initializeActorTable()
{
	actorTable = initializeActorData();
}

Actor* Level::addPlayerActor()
{
	while (true)
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, ActorLayer))
		{
			auto actor = std::make_unique<ActorPlayer>(actorTable[0]);
			actor->setPosition(position);
			Actor* result = actor.get();
			layers[ActorLayer].emplace_back(std::move(actor));

			return result;
		}
	}
}

/*
void Level::addActors(int count)
{
	for (int i = 0; i < count; )
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, ActorLayer))
		{
			layers[ActorLayer].emplace_back(std::make_unique<ActorEnemy>(actorTable[rng.getInt(L'a', L'z') - L'a' + 1]));
			layers[ActorLayer].back()->setPosition(position);
			++i;
		}
	}
}
*/

void Level::addActors(int count, const std::wstring& list)
{
	for (int i = 0; i < count; )
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, ActorLayer))
		{
			wchar_t type = list[rng.getInt(list.size())];
			layers[ActorLayer].emplace_back(std::make_unique<ActorEnemy>(actorTable[type - L'a' + 1]));
			layers[ActorLayer].back()->setPosition(position);
			++i;
		}
	}
}

void Level::addItems(int count)
{
	for (int i = 0; i < count; )
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, ItemLayer))
		{
			Item::Ptr item = nullptr;

			switch (rng.getInt(3))
			{
			case 0: item = std::make_unique<Weapon>(rng.getInt(Weapon::TypeCount)); break;
			case 1: item = std::make_unique<Armor>(rng.getInt(Armor::TypeCount)); break;
			case 2: item = std::make_unique<Medkit>(); break;
			}
			
			item->setPosition(position);
			layers[ItemLayer].emplace_back(std::move(item));
			++i;
		}
	}
}

void Level::addSpaceships(int count)
{
	bool guuuuuum = false;

	for (int i = 0; i < count; )
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, VehicleLayer))
		{
			wchar_t ch = rng.getInt(L'A', L'Z');

			while (ch == L'Q' && guuuuuum)
				ch = rng.getInt(L'A', L'Z');

			if (ch == L'Q')
				guuuuuum = true;

			layers[VehicleLayer].emplace_back(std::make_unique<Vehicle>(ch, rng));
			layers[VehicleLayer].back()->setPosition(position);
			++i;
		}
	}
}

void Level::addPlanets(int count)
{
	for (int i = 0; i < count; )
	{
		sf::Vector2i position = findTile(Location::Random);
		Entity* entity = getEntity(position, PlanetLayer);
		Entity* closest = getClosestEntity(position, PlanetLayer);

		if (entity)
			continue;

		if (!closest || lengthSquared(closest->getPosition() - position) > 2)
		{
			layers[PlanetLayer].emplace_back(std::make_unique<Planet>(0x80, rng));
			layers[PlanetLayer].back()->setPosition(position);
			++i;
		}
	}
}

void Level::addAmulet()
{
	while (true)
	{
		sf::Vector2i position = findTile(Location::Random);

		if (!getEntity(position, ItemLayer))
		{
			auto amulet = std::make_unique<Amulet>(L'*', 0xf0 + rng.getInt(8));
			amulet->setPosition(position);
			layers[ItemLayer].emplace_back(std::move(amulet));

			return;
		}
	}
}

void Level::addBlackHole()
{
	while (true)
	{
		sf::Vector2i position = findTile(Location::Random);
		Entity* entity = getEntity(position, PlanetLayer);
		Entity* closest = getClosestEntity(position, PlanetLayer);

		if (entity)
			continue;

		if (!closest || lengthSquared(closest->getPosition() - position) > 2)
		{
			layers[PlanetLayer].emplace_back(std::make_unique<Planet>(0x80, Planet::BlackHole, rng));
			layers[PlanetLayer].back()->setPosition(position);
			
			return;
		}
	}
}

void Level::removeWalls()
{
	const std::wstring wallTiles = { Tile::Wall, Tile::Unused };

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Tile& tile = at(x, y);

			if (tile.glyph != Tile::Wall)
				continue;

			bool removeWall = true;

			for (auto dir : Direction::All)
			{
				if (isInBounds(x + dir.x, y + dir.y) && (wallTiles.find(at(x + dir.x, y + dir.y).glyph) == std::wstring::npos))
				{
					removeWall = false;
					break;
				}
			}

			if (removeWall)
				tile.glyph = Tile::Unused;
		}
}

void Level::trimWalls()
{
	std::vector<wchar_t> walls(width * height, Tile::Unused);

	const std::wstring wallTiles = { Tile::Wall, Tile::ClosedDoor, Tile::OpenDoor };

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			if (at(x, y).glyph != Tile::Wall)
				continue;

			int value = 0;

			if (y > 0 && wallTiles.find(at(x, y - 1).glyph) != std::wstring::npos)
				value += 1;
			if (x > 0 && wallTiles.find(at(x - 1, y).glyph) != std::wstring::npos)
				value += 2;
			if (x < width - 1 && wallTiles.find(at(x + 1, y).glyph) != std::wstring::npos)
				value += 4;
			if (y < height - 1 && wallTiles.find(at(x, y + 1).glyph) != std::wstring::npos)
				value += 8;

			walls[x + y * width] = value;
		}

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			int tile = walls[x + y * width];

			if (tile == Tile::Unused)
				continue;

			/*
			// Light
			switch (tile)
			{
			case  0: tile = L'#'; break;
			case  1: tile = L'¦¢'; break; // 0x2502
			case  2: tile = L'¦¡'; break; // 0x2500
			case  3: tile = L'¦¥'; break; // 0x2518
			case  4: tile = L'¦¡'; break; // 0x2500
			case  5: tile = L'¦¦'; break; // 0x2514
			case  6: tile = L'¦¡'; break; // 0x2500
			case  7: tile = L'¦ª'; break; // 0x2534
			case  8: tile = L'¦¢'; break; // 0x2502
			case  9: tile = L'¦¢'; break; // 0x2502
			case 10: tile = L'¦¤'; break; // 0x2510
			case 11: tile = L'¦©'; break; // 0x2524
			case 12: tile = L'¦£'; break; // 0x250c
			case 13: tile = L'¦§'; break; // 0x251c
			case 14: tile = L'¦¨'; break; // 0x252c
			case 15: tile = L'¦«'; break; // 0x253c
			}
			*/

			// TODO: Heavy

			// Double
			switch (tile)
			{
			case  0: tile = L'#'; break;
			case  1: tile = 0x2551; break; // L'¦¢';
			case  2: tile = 0x2550; break; // L'¦¡';
			case  3: tile = 0x255d; break; // L'¦¥';
			case  4: tile = 0x2550; break; // L'¦¡';
			case  5: tile = 0x255a; break; // L'¦¦';
			case  6: tile = 0x2550; break; // L'¦¡';
			case  7: tile = 0x2569; break; // L'¦ª';
			case  8: tile = 0x2551; break; // L'¦¢';
			case  9: tile = 0x2551; break; // L'¦¢';
			case 10: tile = 0x2557; break; // L'¦¤';
			case 11: tile = 0x2563; break; // L'¦©';
			case 12: tile = 0x2554; break; // L'¦£';
			case 13: tile = 0x2560; break; // L'¦§';
			case 14: tile = 0x2566; break; // L'¦¨';
			case 15: tile = 0x256c; break; // L'¦«';
			}

			at(x, y).glyph = tile;

			// HACK: Unicode to CP437
			int number = L'#';

			switch (tile)
			{
			case L'¦¢': case 0x2551: number = 179; break; // 0x2502
			case L'¦©': case 0x2563: number = 180; break; // 0x2524
			case L'¦¤': case 0x2557: number = 191; break; // 0x2510
			case L'¦¦': case 0x255a: number = 192; break; // 0x2514
			case L'¦ª': case 0x2569: number = 193; break; // 0x2534
			case L'¦¨': case 0x2566: number = 194; break; // 0x252c
			case L'¦§': case 0x2560: number = 195; break; // 0x251c
			case L'¦¡': case 0x2550: number = 196; break; // 0x2500
			case L'¦«': case 0x256c: number = 197; break; // 0x253c
			case L'¦¥': case 0x255d: number = 217; break; // 0x2518
			case L'¦£': case 0x2554: number = 218; break; // 0x250c
			}

			at(x, y).tileNumber = number;
			//
		}

	// TODO: advanced trim
}