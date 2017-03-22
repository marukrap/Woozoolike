#pragma once

#include "Level.hpp"

struct PlanetInfo;

// TODO: check map validity, place more treasures

class Ground : public Level
{
public:
	Ground(const sf::Vector2i& size, Rng& rng, const PlanetInfo& info);

	LevelType getLevelType() const override;
	sf::Vector2i findTile(Location location) const override;

private:
	void onGenerate() override;
	void onDecorate() override;
	void onInitialize() override;

	void generation(int r1cutoff, int r2cutoff = -1);

private:
	const PlanetInfo& info;

	// HACK: used for BlackHole
	sf::Vector2i whiteHole;
	sf::Vector2i singularity;
	// sf::Vector2i landingPoint;
};