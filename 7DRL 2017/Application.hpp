#pragma once

#include "Renderer.hpp"
#include "StateStack.hpp"
#include "Fps.hpp"
#include "Config.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <memory> // unique_ptr

class Application
{
public:
	Application();

	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void initializeRenderer(int width, int height, Renderer::Type type);
	void toggleVideoFilter();
	void zoom(float factor);

private:
	// Window
	sf::RenderWindow window;
	sf::View view;
	sf::Vector2f viewSize;
	float viewScale = 1.f;

	// Resources
	sf::Font font;
	std::unique_ptr<sf::Font> koreanFont = nullptr;
	std::unique_ptr<sf::Texture> texture = nullptr;
	std::unique_ptr<sf::Sprite> filterSprite = nullptr;

	//
	std::unique_ptr<Renderer> renderer = nullptr;
	std::unique_ptr<Renderer> uiRenderer = nullptr;
	StateStack stateStack;
	Fps fps;
	Config config;

	// bool mousePressed = false;
	// sf::Vector2i mousePosition;
};