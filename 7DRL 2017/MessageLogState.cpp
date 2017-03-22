#include "MessageLogState.hpp"
#include "Renderer.hpp"
#include "World.hpp"
#include "Color.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

MessageLogState::MessageLogState(StateStack& stack)
	: State(stack)
	, renderer(*getContext().uiRenderer)
{
	renderer.clear();
	renderer.setColor(0, 0, renderer.getSize().x, renderer.getSize().y, getContext().world->getBackgroundColor());
	renderer.setBox(0, 0, renderer.getSize().x, renderer.getSize().y, Color::White);

	MessageLog& log = getContext().world->getLog();

	for (std::size_t i = 0; i < log.messages.size(); ++i)
	{
		MessageLog::Message& msg = log.messages[i];

		if (msg.repeat > 0)
			renderer.setString(2, 1 + i, msg.string + L" (x" + std::to_wstring(msg.repeat + 1) + L")", msg.color);
		else
			renderer.setString(2, 1 + i, msg.string, msg.color);
	}
}

bool MessageLogState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M)
			requestStackPop();
	}

	return false;
}

bool MessageLogState::update(sf::Time dt)
{
	return true;
}

void MessageLogState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(renderer);
}