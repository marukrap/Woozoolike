#pragma once

namespace sf
{
	class RenderWindow;
	class View;
}

class Renderer;
class World;
struct Config;

struct Context
{
	sf::RenderWindow* window;
	sf::View* view;
	Renderer* renderer;
	Renderer* uiRenderer;
	World* world;
	Config* config;
};