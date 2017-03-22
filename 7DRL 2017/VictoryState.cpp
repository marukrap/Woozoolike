#include "VictoryState.hpp"
#include "Renderer.hpp"
#include "World.hpp"
#include "Strings.hpp"
#include "Color.hpp"

#include "Actor.hpp" // HACK: victory

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


VictoryState::VictoryState(StateStack& stack)
	: State(stack)
	, renderer(*getContext().uiRenderer)
{
	renderer.clear();

	Level& galaxy = getContext().world->getGalaxy();

	for (auto& entity : galaxy[Level::VehicleLayer])
	{
		if (!entity->getLevel())
			achievement &= ~SpacePirate;
	}

	for (auto& entity : galaxy[Level::PlanetLayer])
	{
		if (!entity->getLevel())
			achievement &= ~PlanetExplorer;
	}
}

bool VictoryState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			requestStackPush("Pause");
			dirty = true;
		}
	}

	return false;
}

bool VictoryState::update(sf::Time dt)
{
	if (dirty)
	{
		dirty = false;

		renderer.clear();
		renderer.setColor(0, 0, renderer.getSize().x, renderer.getSize().y, getContext().world->getBackgroundColor());

		int y = 2;

		if (getContext().world->getPlayerActor().victory == 1)
		{
			renderer.setString(3, y++, Strings::get("Victory"));
			renderer.setString(3, y++, Strings::get("Victory2"));
		}

		else // victory == 2
		{
			renderer.setString(3, y++, Strings::get("Victory3"));
			renderer.setString(3, y++, Strings::get("Victory4"));
		}

		y++;

		renderer.setString(3, y++, Strings::get("YouWon"), Color::Blue);

		y++;

		if (achievement & SpacePirate)
			renderer.setString(3, y++, Strings::get("SpacePirate"), Color::Blue);

		if (achievement & PlanetExplorer)
			renderer.setString(3, y++, Strings::get("PlanetExplorer"), Color::Blue);

		// TODO: highscore, enemies killed, spacehips invaded, planets explored, the amulet pieces collected, ...
	}

	return false;
}

void VictoryState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(renderer);
}