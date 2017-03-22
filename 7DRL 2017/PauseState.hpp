#pragma once

#include "State.hpp"

#include <SFML/Graphics/Color.hpp>

#include <string>

class Renderer;

class PauseState : public State
{
public:
	explicit PauseState(StateStack& stack);

	bool handleEvent(const sf::Event& event) override;
	bool update(sf::Time dt) override;
	void draw() override;

private:
	void printCenter(int y, const std::wstring& string, const sf::Color& color);

private:
	Renderer& renderer;
	bool dirty = true;
	int selected = 0;
};