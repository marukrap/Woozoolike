#include "Projectile.hpp"
#include "Actor.hpp"
#include "World.hpp"
#include "Utility.hpp" // plotPath

#include <cassert>

Projectile::Projectile(Actor& actor, std::vector<sf::Vector2i> path)
	: Entity(L'\\', 0xea)
	, actor(actor)
	, path(path)
{
	sf::Vector2i dir = path.front() - actor.getPosition();
	
	if (dir.x != 0 && dir.y == 0)
	{
		glyph = L'-';
		tileNumber = 0xeb;
	}

	else if (dir.x == 0 && dir.y != 0)
	{
		glyph = L'|';
		tileNumber = 0xec;
	}

	else if (dir.x + dir.y == 0)
	{
		glyph = L'/';
		tileNumber = 0xed;
	}

	assert(dir.x != 0 || dir.y != 0);

	setPosition(path.front());
}

EntityType Projectile::getEntityType() const
{
	return EntityType::Projectile;
}

std::wstring Projectile::getName(Article article) const
{
	std::wstring name = L"projectile"; // unused

	return attachArticle(name, article);
}

Actor& Projectile::getActor() const
{
	return actor;
}

bool Projectile::isFinished() const
{
	return path.empty();
}

void Projectile::update(sf::Time dt)
{
	//* Projectile delay
	static const sf::Time delay = sf::seconds(0.02f);

	elapsedTime += dt;

	if (elapsedTime < delay)
		return;

	elapsedTime -= delay;
	//*/

	if (path.empty())
		return;

	path.erase(path.begin());

	// REMOVE: dynamic_cast
	Actor* target = dynamic_cast<Actor*>(world->getLevel().getEntity(getPosition(), Level::ActorLayer));

	if (target && !target->isDestroyed() && target->getActorType() != actor.getActorType())
	{
		actor.attack(*target, true);
		path.clear();
	}

	if (!path.empty())
		setPosition(path.front());
}

void ProjectileQueue::add(Actor& actor, const sf::Vector2i& target, int count)
{
	assert(actor.getPosition() != target);

	auto path = plotPath(actor.getPosition(), target);

	for (int i = 0; i < count - 1; ++i)
		projectiles.emplace_back(std::make_unique<Projectile>(actor, path));

	projectiles.emplace_back(std::make_unique<Projectile>(actor, std::move(path)));
}

void ProjectileQueue::add(Actor& actor, std::vector<sf::Vector2i> path, int count)
{
	for (int i = 0; i < count - 1; ++i)
		projectiles.emplace_back(std::make_unique<Projectile>(actor, path));

	projectiles.emplace_back(std::make_unique<Projectile>(actor, std::move(path)));
}

bool ProjectileQueue::isEmpty() const
{
	return projectiles.empty();
}

void ProjectileQueue::update(sf::Time dt)
{
	if (projectiles.empty())
		return;

	if (projectiles.front()->isFinished())
	{
		Actor& actor = projectiles.front()->getActor();
		projectiles.erase(projectiles.begin());

		if (projectiles.empty())
		{
			// HACK: end player turn
			if (actor.getActorType() == ActorType::Player)
				world->endPlayerTurn();
		}
	}

	else
		projectiles.front()->update(dt);

	world->dirty();
}

void ProjectileQueue::draw(Renderer& renderer)
{
	if (!projectiles.empty())
		projectiles.front()->draw(renderer);
}