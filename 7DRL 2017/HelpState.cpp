#include "HelpState.hpp"
#include "Renderer.hpp"
#include "World.hpp" // getBackgroundColor
#include "Strings.hpp"
#include "Color.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

HelpState::HelpState(StateStack& stack)
	: State(stack)
	, renderer(*getContext().uiRenderer)
{
	renderer.clear();
	renderer.setColor(0, 0, renderer.getSize().x, renderer.getSize().y, getContext().world->getBackgroundColor());

	int x = 11;
	int y = 0;

	y++;
	renderer.setString(x, y++, Strings::get("Commands"), Color::LightGray);

	y++;
	printCommand(x, y++, L"789  yku", Strings::get("Command"));
	printCommand(x, y++, L"4 6  h l", Strings::get("Command2"));
	printCommand(x, y++, L"123  bjn", Strings::get("Command3"));
	
	y++;
	printCommand(x, y++, L"Space", Strings::get("Command4"));
	printCommand(x, y++, L"e", Strings::get("Command5"));
	printCommand(x, y++, L"m", Strings::get("Command6"));

	// s - search (activate a radar)
	// i - describe equipped weapon

	y++;
	printCommand(x, y++, L"q/w", Strings::get("Command7"));
	printCommand(x, y++, L"d", Strings::get("Command8"));
	printCommand(x, y++, L"f", Strings::get("Command9"));
	
	y++;
	renderer.setString(x, y++, Strings::get("Functions"), Color::LightGray);

	y++;
	printCommand(x, y++, L"+/-", Strings::get("Zoom"));

	y++;
	printCommand(x, y++, L"F1", Strings::get("F1"));
	printCommand(x, y++, L"F2", Strings::get("F2"));
	printCommand(x, y++, L"F3", Strings::get("F3"));

	// F4 - toggle camera mode

	#ifdef _DEBUG
	printCommand(x, y++, L"F5", L"toggle field of view (debug mode)");
	printCommand(x, y++, L"F6", L"change fov algorithm (debug mode)");
	#endif
}

bool HelpState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::F1)
			requestStackPop();

		else if (event.key.code == sf::Keyboard::Slash)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				requestStackPop();
		}
	}

	return false;
}

bool HelpState::update(sf::Time dt)
{
	return true;
}

void HelpState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(renderer);
}

void HelpState::printCommand(int x, int y, const std::wstring& command, const std::wstring& description)
{
	renderer.setString(x, y, command, Color::Yellow);
	renderer.setString(x + 11, y, description, Color::White);
}