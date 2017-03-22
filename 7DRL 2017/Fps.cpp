#include "Fps.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

void Fps::setFont(const sf::Font& font, unsigned int fontSize)
{
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setOutlineThickness(2.f);
}

void Fps::update(sf::Time dt)
{
	elapsedTime += dt;
	numFrames += 1;

	if (elapsedTime >= sf::seconds(1.f))
	{
		text.setString(std::to_string(numFrames));
		elapsedTime -= sf::seconds(1.f);
		numFrames = 0;
	}
}

void Fps::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(text, states);
}