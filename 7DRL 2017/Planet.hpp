#pragma once

#include "Entity.hpp"

class Level;
class Rng;

// HACK:
struct PlanetInfo
{
	int theme;
	int wallprob;
	int tileNumber;
	sf::Color floorColor;
	sf::Color wallColor;
};

class Planet : public Entity
{
public:
	using Ptr = std::unique_ptr<Planet>;

	enum Theme
	{
		Grassy, // Forest, Jungle or Terra
		Desert,
		Burning,
		Snowy, // Ice World
		Ocean, // Oceania
		BlackHole, // Moon
		ThemeCount
	};

public:
	Planet(int tileNumber, Rng& rng);
	Planet(int tileNumber, Theme theme, Rng& rng);

	EntityType getEntityType() const override;

	std::wstring getName(Article article = Article::None) const override;
	const PlanetInfo& getInfo() const;

	void attachLevel(std::unique_ptr<Level> level);
	Level* getLevel() const;

	void draw(Renderer& renderer) override;

private:
	void assignGlyph(int tileNumber);
	void generateName(Rng& rng);

private:
	std::unique_ptr<Level> level = nullptr;
	std::wstring name;
	Theme theme;
};