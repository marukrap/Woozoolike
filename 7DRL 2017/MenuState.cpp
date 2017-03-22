#include "MenuState.hpp"

// #include <SFML/Graphics/RenderWindow.hpp>
// #include <SFML/Window/Event.hpp>

MenuState::MenuState(StateStack& stack)
	: State(stack)
{
	// TODO: select your language (English or Korean)
}

bool MenuState::handleEvent(const sf::Event& event)
{
	return false;
}

bool MenuState::update(sf::Time dt)
{
	return false;
}

void MenuState::draw()
{
	// sf::RenderWindow& window = *stack.getContext().window;
}