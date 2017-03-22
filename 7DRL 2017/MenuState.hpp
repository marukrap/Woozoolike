#pragma once

#include "State.hpp"

class MenuState : public State
{
public:
	explicit MenuState(StateStack& stack);

	bool handleEvent(const sf::Event& event) override;
	bool update(sf::Time dt) override;
	void draw() override;
};