#pragma once

#include "Entity.hpp"

#include <SFML/System/Time.hpp>

#include <vector>

class Actor;

class Projectile : public Entity
{
public:
	using Ptr = std::unique_ptr<Projectile>;

public:
	Projectile(Actor& actor, std::vector<sf::Vector2i> path);
	
	EntityType getEntityType() const override;

	std::wstring getName(Article article = Article::None) const override;
	Actor& getActor() const;

	bool isFinished() const;

	void update(sf::Time dt);

private:
	Actor& actor; // thrower
	std::vector<sf::Vector2i> path;
	sf::Time elapsedTime;
};

class ProjectileQueue : public GameObject
{
public:
	void add(Actor& actor, const sf::Vector2i& target, int count = 1);
	void add(Actor& actor, std::vector<sf::Vector2i> path, int count = 1);

	bool isEmpty() const;

	void update(sf::Time dt);

	void draw(Renderer& renderer);

private:
	std::vector<Projectile::Ptr> projectiles;
};