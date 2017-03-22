#pragma once

// TODO: consume more fuel when landing or taking off
// TODO: more vehicles (rover, tank)

#include "Entity.hpp"

class Actor;
class Level;
class Rng;

class Vehicle : public Entity
{
public:
	using Ptr = std::unique_ptr<Vehicle>;

public:
	Vehicle(wchar_t ch, Rng& rng);

	EntityType getEntityType() const override;

	std::wstring getName(Article article = Article::None) const override;

	void changeFuel(int amount);
	int getFuel() const;
	int getMaxFuel() const;

	void attachLevel(std::unique_ptr<Level> level);
	Level* getLevel() const;

	void draw(Renderer& renderer) override;

private:
	void generateName(Rng& rng);

private:
	int fuel;
	int maxFuel;

	std::unique_ptr<Level> level = nullptr;
	std::wstring name;
};