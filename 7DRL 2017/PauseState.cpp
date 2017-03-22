#include "PauseState.hpp"
#include "Renderer.hpp"
#include "World.hpp" // getBackgroundColor
#include "Strings.hpp"
#include "Color.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

PauseState::PauseState(StateStack& stack)
	: State(stack)
	, renderer(*getContext().uiRenderer)
{
	renderer.clear();
}

bool PauseState::handleEvent(const sf::Event& event)
{
	const int maxSelected = 3;

	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			requestStackPop();
			break;

		case sf::Keyboard::Up:
		case sf::Keyboard::Numpad8:
		case sf::Keyboard::K:
			selected = (selected + maxSelected - 1) % maxSelected;
			dirty = true;
			break;

		case sf::Keyboard::Down:
		case sf::Keyboard::Numpad2:
		case sf::Keyboard::J:
			selected = (selected + 1) % maxSelected;
			dirty = true;
			break;

		case sf::Keyboard::Return:
		case sf::Keyboard::Space:
			switch (selected)
			{
			case 0: // Return
				requestStackPop();
				break;

			case 1: // Restart
				requestStateClear();
				requestStackPush("Game");
				break;

			case 2: // Quit
				requestStateClear();
				break;
			}
			break;
		}
	}

	return false;
}

bool PauseState::update(sf::Time dt)
{
	if (dirty)
	{
		dirty = false;

		renderer.clear();
		renderer.setColor(0, 0, renderer.getSize().x, renderer.getSize().y, getContext().world->getBackgroundColor());

		int x = renderer.getSize().x / 2;
		int y = renderer.getSize().y / 2;

		printCenter(y++, Strings::get("Return"), selected == 0 ? Color::Red : Color::White);
		printCenter(y++, Strings::get("Restart"), selected == 1 ? Color::Red : Color::White);
		printCenter(y++, Strings::get("Quit"), selected == 2 ? Color::Red : Color::White);
	}

	return false;
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(renderer);
}

void PauseState::printCenter(int y, const std::wstring& string, const sf::Color& color)
{
	renderer.setString(renderer.getSize().x / 2 - string.size() / 2, y, string, color);
}