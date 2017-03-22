#pragma once

#include "State.hpp"

class Renderer;

class VictoryState : public State
{
public:
	enum Achievement
	{
		None			= 0,
		SpacePirate		= 1 << 0,
		PlanetExplorer	= 1 << 1,
		All				= SpacePirate | PlanetExplorer
	};

public:
	explicit VictoryState(StateStack& stack);

	bool handleEvent(const sf::Event& event) override;
	bool update(sf::Time dt) override;
	void draw() override;

private:
	Renderer& renderer;
	bool dirty = true;
	unsigned int achievement = All;
};