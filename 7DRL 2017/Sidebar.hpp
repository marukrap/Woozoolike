#pragma once

#include "Console.hpp"
#include "GameObject.hpp"

struct Config;
class Actor;

class Sidebar : public Console, public GameObject
{
public:
	explicit Sidebar(Config& config);

	void refresh(Actor& actor);

private:
	void drawLine(int y);

	void printCenter(int y, const std::wstring& string, const sf::Color& color);
	void printCommand(int x, int y, const std::wstring& command, const std::wstring& description);
};