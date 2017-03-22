#pragma once

#include "Level.hpp"

class Galaxy : public Level
{
public:
	Galaxy(const sf::Vector2i& size, Rng& rng);

	LevelType getLevelType() const override;

private:
	void onGenerate() override;
	void onDecorate() override;
	void onInitialize() override;

	void generateName(Rng& rng);
};