#pragma once

#include "Equipment.hpp"
#include "AI.hpp"

enum class ActorType
{
	Player,
	Enemy,
	Ally,
};

/*
enum class ActorState
{
	Walking,
	Running,
	Fighting, // Attacking, Firing
	Resting, // Waiting
	Wondering,
};
*/

struct ActorData;
class Vehicle;
class Planet;

class Actor : public Entity
{
public:
	using Ptr = std::unique_ptr<Actor>;

public:
	explicit Actor(ActorData& data);

	EntityType getEntityType() const override;
	virtual ActorType getActorType() const = 0;

	std::wstring getName(Article article = Article::None) const override;

	bool isDestroyed() const override;

	using Entity::setPosition;
	using Entity::move;

	void setPosition(int x, int y) override;
	void move(int dx, int dy) override;

	void attack(Actor& actor, bool ranged);

	void takeDamage(int points);
	void restoreHp(int points);

	void changeMaxHp(int points);
	int getHp() const;
	int getMaxHp() const;

	void changeO2(int oxygen);
	int getO2() const;
	int getMaxO2() const;

	int getDamage() const;

	void finishTurn();
	
	void setLastTarget(Actor* target);
	Actor* getLastTarget() const;

	// ActorPlayer
	void setVehicle(Vehicle* vehicle);
	Vehicle* getVehicle() const;

	// ActorMonster
	void updateAI();

	void draw(Renderer& renderer) override;

public:
	Equipment equipment;

protected:
	ActorData& data;

	Actor* lastTarget = nullptr;
	Vehicle* vehicle = nullptr;
	std::unique_ptr<AI> ai = nullptr;

	int hp;
	int maxHp;
	int o2;
	int maxO2;
	int regen = 0;
	// int fuel = 0; // supply fuel when you return to the ship

	friend class Combat;

public:
	// REMOVE: temporary
	int amulet = 0;
	int victory = 0; // 1: escape, 2: victory
};