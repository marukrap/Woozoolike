#pragma once

#include "State.hpp"

#include <string>

class Renderer;

class HelpState : public State
{
public:
	explicit HelpState(StateStack& stack);

	bool handleEvent(const sf::Event& event) override;
	bool update(sf::Time dt) override;
	void draw() override;

private:
	void printCommand(int x, int y, const std::wstring& command, const std::wstring& description);

private:
	Renderer& renderer;
};