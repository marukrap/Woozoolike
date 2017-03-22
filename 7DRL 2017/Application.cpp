#include "Application.hpp"

// Renderer
#include "Console.hpp"
#include "TileMap.hpp"

// States
// #include "MenuState.hpp"
#include "GameState.hpp"
#include "HelpState.hpp"
#include "PauseState.hpp"
#include "MessageLogState.hpp"
#include "VictoryState.hpp"

// Resources
#include "Resources/Fantasque_ttf.cpp"
#include "Resources/Square_ttf.cpp"
#include "Resources/Tileset_png.cpp"

#include "Strings.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>

Application::Application()
	: stateStack({ nullptr, nullptr, nullptr, nullptr, nullptr, &config })
{
	// font.loadFromFile("Fonts/Fantasque.ttf");
	font.loadFromMemory(fantasque_ttf, fantasque_ttf_length);

	fps.setFont(font, 14u);
	config.font = &font;

	initializeRenderer(config.mapSize.x, config.mapSize.y, Renderer::Ascii);

	// initialize sf::RenderWindow
	sf::Vector2i size = config.screenSize;
	sf::Vector2f tileSize = renderer->getTileSize();

	unsigned int modeWidth = static_cast<unsigned int>(size.x * tileSize.x);
	unsigned int modeHeight = static_cast<unsigned int>(size.y * tileSize.y);
	std::cout << "Window Size = " << modeWidth << " x " << modeHeight << std::endl;

	window.create(sf::VideoMode(modeWidth, modeHeight), "Woozoolike", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	
	// initialize sf::View
	viewSize.x = config.viewSize.x * tileSize.x;
	viewSize.y = config.viewSize.y * tileSize.y;

	sf::FloatRect viewport;
	viewport.width = static_cast<float>(config.viewSize.x) / config.screenSize.x;
	viewport.height = static_cast<float>(config.viewSize.y) / config.screenSize.y;

	view = window.getDefaultView();
	view.setSize(viewSize);
	view.setViewport(viewport);

	stateStack.getContext().window = &window;
	stateStack.getContext().view = &view;

	// initialize video filter
	toggleVideoFilter();

	// register states
	// stateStack.registerState<MenuState>("Menu");
	stateStack.registerState<GameState>("Game");
	stateStack.registerState<HelpState>("Help");
	stateStack.registerState<PauseState>("Pause");
	stateStack.registerState<MessageLogState>("MessageLog");
	stateStack.registerState<VictoryState>("Victory");

	stateStack.pushState("Game");

	// REMOVE: the renderer is initialized twice
	// TODO: initialize the renderer the view size
	initializeRenderer(config.mapSize.x, config.mapSize.y, Renderer::Tile);

	// HACK: UI renderer for Help/Pause/MessageLog State
	uiRenderer = std::make_unique<Console>(config.viewSize.x, config.viewSize.y, *config.font, config.fontSize);
	stateStack.getContext().uiRenderer = uiRenderer.get();

	// HACK: Korean
	if (Strings::getLanguage() == Language::Korean && !koreanFont)
	{
		koreanFont = std::make_unique<sf::Font>();
		// koreanFont.loadFromFile("Fonts/Square.ttf");
		koreanFont->loadFromMemory(square_ttf, square_ttf_length);
		Console::setKoreanFont(*koreanFont, 19);
	}
}

void Application::run()
{
	static const sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate;

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		if (timeSinceLastUpdate >= timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			processInput();
			update(timePerFrame);

			if (stateStack.isEmpty())
				window.close();
		}

		fps.update(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::F2:
				initializeRenderer(renderer->getSize().x, renderer->getSize().y, static_cast<Renderer::Type>((renderer->getType() + 1) % Renderer::TypeCount));
				break;

			case sf::Keyboard::F3:
				toggleVideoFilter();
				break;

			// TODO: move to GameState
			case sf::Keyboard::Add:
			case sf::Keyboard::Equal:
				if (viewScale < 4.f)
				{
					if (viewScale > 1.f)
						zoom(viewScale / (viewScale + 1));
					else
						zoom(0.5f);

					if (stateStack.getContext().world)
						stateStack.getContext().world->dirty();
				}
				break;

			case sf::Keyboard::Subtract:
			case sf::Keyboard::Dash:
				if (viewScale > 0.25f)
				{
					if (viewScale > 1.f)
						zoom(viewScale / (viewScale - 1));
					else
						zoom(2.f);

					if (stateStack.getContext().world)
						stateStack.getContext().world->dirty();
				}
				break;
			//
			}
		}

		/*
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			mousePressed = true;
			mousePosition.x = event.mouseButton.x;
			mousePosition.y = event.mouseButton.y;
		}

		else if (event.type == sf::Event::MouseButtonReleased)
		{
			mousePressed = false;
		}

		else if (event.type == sf::Event::MouseMoved)
		{
			if (mousePressed)
			{
				sf::Vector2i newMousePosition(event.mouseMove.x, event.mouseMove.y);

				view.move(sf::Vector2f(mousePosition - newMousePosition) / viewScale);
				mousePosition = newMousePosition;
			}
		}

		else if (event.type == sf::Event::MouseWheelScrolled)
		{
			float factor = event.mouseWheelScroll.delta > 0.f ? 0.5f : 2.f;
			sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			sf::Vector2f center = view.getCenter() * factor + mousePosition * (1.f - factor);

			zoom(factor);
			view.setCenter(std::floor(center.x), std::floor(center.y));
		}
		*/

		stateStack.handleEvent(event);
	}
}

void Application::update(sf::Time dt)
{
	stateStack.update(dt);
}

void Application::render()
{
	// HACK: BlockHole
	if (stateStack.getContext().world)
		window.clear(stateStack.getContext().world->getClearColor());
	else
		window.clear(sf::Color(19, 23, 28)); // Color::Black

	stateStack.draw();
	#ifdef _DEBUG
	window.draw(fps);
	#endif
	if (filterSprite)
		window.draw(*filterSprite);
	window.display();
}

void Application::initializeRenderer(int width, int height, Renderer::Type type)
{
	if (type == Renderer::Ascii)
	{
		// Fira - 20 / Fantasque - 23
		renderer = std::make_unique<Console>(width, height, font, config.fontSize);

		std::cout << "Renderer: ASCII" << std::endl;
	}

	else // Renderer::Tile
	{
		if (!texture)
		{
			/*
			sf::Image image;
			image.loadFromFile("Tileset.png");
			image.createMaskFromColor(sf::Color(0, 0, 0, 255));

			texture = std::make_unique<sf::Texture>();
			texture->loadFromImage(image);
			*/

			texture = std::make_unique<sf::Texture>();
			// texture->loadFromFile("Textures/Tileset_Oryx.png");
			texture->loadFromMemory(tileset_png, tileset_png_length);
		}

		renderer = std::make_unique<TileMap>(width, height, *texture, config.tileSize);
		
		std::cout << "Renderer: Graphics" << std::endl;
	}

	stateStack.getContext().renderer = renderer.get();

	if (stateStack.getContext().world)
		stateStack.getContext().world->dirty();
}

void Application::toggleVideoFilter()
{
	static std::unique_ptr<sf::Texture> filterTexture = nullptr;
	static const int intensity = 50;

	if (filterSprite)
	{
		filterSprite = nullptr;

		std::cout << "Video Filter: Off" << std::endl;
		return;
	}

	if (!filterTexture)
	{
		sf::Image image;
		image.create(2, 2);

		for (unsigned int y = 0; y < image.getSize().y; ++y)
			for (unsigned int x = 0; x < image.getSize().x; ++x)
			{
				sf::Color color = sf::Color::Black;
				color.a = intensity * (y % 2);

				image.setPixel(x, y, color);
			}

		filterTexture = std::make_unique<sf::Texture>();
		filterTexture->loadFromImage(image);
		filterTexture->setRepeated(true);
	}

	// sf::IntRect textureRect;
	// textureRect.width = static_cast<int>(window.getSize().x / viewScale);
	// textureRect.height = static_cast<int>(window.getSize().y / viewScale);

	sf::IntRect textureRect(0, 0, window.getSize().x, window.getSize().y);

	filterSprite = std::make_unique<sf::Sprite>(*filterTexture, textureRect);
	// filterSprite->setScale(viewScale, viewScale);

	std::cout << "Video Filter: On" << std::endl;
}

void Application::zoom(float factor)
{
	view.zoom(factor);
	viewScale /= factor;

	/*
	if (filterSprite)
	{
		sf::IntRect textureRect;
		textureRect.width = static_cast<int>(window.getSize().x / viewScale);
		textureRect.height = static_cast<int>(window.getSize().y / viewScale);

		filterSprite->setTextureRect(textureRect);
		filterSprite->setScale(viewScale, viewScale);
	}
	*/

	std::cout << "Scale: " << viewScale * 100.f << "%" << std::endl;
}