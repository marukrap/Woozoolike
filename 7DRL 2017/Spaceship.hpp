#pragma once

#include "Level.hpp"

class Spaceship : public Level
{
public:
	struct Room
	{
		int right() const;
		int bottom() const;

		int x, y, width, height;
	};

	enum RoomType
	{
		Storeroom, // obstacles
		Bedroom,
		Library,
		SleepPod,
		TypeCount
	};

public:
	using Level::Level;
	
	LevelType getLevelType() const override;
	sf::Vector2i findTile(Location location) const override;

private:
	void onGenerate() override;
	void onDecorate() override;
	void onInitialize() override;

	void colorize();

	// generate
	void addRooms(int tries);

	bool canCarve(const Room& room) const;
	void carve(const Room& room);

	bool isCarved(int x, int y) const;
	void connect(const Room& room);

	// decorate
	void decorate(Room& room);
	bool decorateFloor(Room& room);
	void decorateHatch(Room& room);
	void decorateCockpit(Room& room);

	void placeObjects(Room& room);

	bool blocksDoor(int x, int y) const;
	bool placeObject(int x, int y, wchar_t glyph, int tileNumber, bool passable = false, bool transparent = false, const sf::Color& color = sf::Color::White);

	bool placeObstacles(int x, int y);
	bool placeBed(int x, int y);

	bool decorateWindows(Room& room);
	bool placeWindow(int x, int y, const sf::Vector2i& dir, wchar_t glyph, int tileNumber);

	void carveCorners(const Room& room);
	void carveCorner(const Room& room, int x, int y, const sf::Vector2i& dir);

private:
	std::vector<Room> rooms;
	std::array<int, TypeCount> counters;
	sf::Vector2i hatch;
	sf::Vector2i cockpit;
};