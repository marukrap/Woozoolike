#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

// FPS counter
class Fps : public sf::Drawable, public sf::Transformable
{
public:
	void setFont(const sf::Font& font, unsigned int fontSize);

	void update(sf::Time dt);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Text text;
	sf::Time elapsedTime;
	std::size_t numFrames = 0;
};