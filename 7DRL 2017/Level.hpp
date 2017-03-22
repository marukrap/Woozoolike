#pragma once

#include "Map.hpp"
#include "Entity.hpp"

#include <array>

enum class LevelType
{
	Galaxy,
	Spaceship,
	Planet,
	BlackHole,
};

enum class Location
{
	Random,
	Hatch, // Spaceship
	Cockpit, // Spaceship
	WhiteHole, // BlackHole
	Singularity, // BlackHole
};

class Rng;
class Actor;

class Level : public Map
{
public:
	using Ptr = std::unique_ptr<Level>;
	using EntityLayer = std::vector<Entity::Ptr>;

	enum Layer
	{
		PlanetLayer,
		ItemLayer,
		VehicleLayer,
		ActorLayer,
		LayerCount
	};

public:
	Level(const sf::Vector2i& size, Rng& rng);
	
	void build();

	virtual LevelType getLevelType() const = 0;
	virtual sf::Vector2i findTile(Location location) const;

	void setName(const std::wstring& name);
	const std::wstring& getName() const;

	std::vector<Entity::Ptr>& operator[](Layer layer);
	const std::vector<Entity::Ptr>& operator[](Layer layer) const;

	Entity* getEntity(const sf::Vector2i& position, Layer layer);
	Entity* getClosestEntity(const sf::Vector2i& position, Layer layer);
	std::vector<Entity*> getVisibleEntities(Layer layer);

	void attachEntity(Entity::Ptr entity, Layer layer);
	Entity::Ptr detachEntity(Entity& entity, Layer layer);
	void removeWrecks(Layer layer = ActorLayer);

	void draw(Renderer& renderer) override;

	static void initializeActorTable();

	Actor* addPlayerActor();

protected:
	// void addActors(int count);
	void addActors(int count, const std::wstring& list);
	void addItems(int count);
	void addSpaceships(int count);
	void addPlanets(int count);

	void addAmulet();
	void addBlackHole();

	void removeWalls();
	void trimWalls();

private:
	virtual void onGenerate() = 0;
	virtual void onDecorate() = 0;
	virtual void onInitialize() = 0;

protected:
	Rng& rng;
	std::wstring name;
	std::array<EntityLayer, LayerCount> layers;
};