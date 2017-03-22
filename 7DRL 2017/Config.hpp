#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Font;
}

// namespace Config
struct Config : public sf::NonCopyable
{
	const sf::Vector2i screenSize = { 100, 30 };
	const sf::Vector2i viewSize = { 80, 25 };

	const sf::Vector2i mapSize = { 80, 50 };

	// UI
	const sf::Vector2i sidebarSize = { 20, 30 };
	const sf::Vector2i logSize = { 80, 5 };

	// UI font
	const sf::Font* font = nullptr;
	const int fontSize = 23;

	// tileset
	const sf::Vector2f tileSize = { 24.f, 24.f };

	// field of view range
	const int fov = 10;
};