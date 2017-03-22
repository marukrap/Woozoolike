#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <cmath> // sqrt, abs

// Random number generation
int randomInt(int exclusiveMax); // [0, max)
int randomInt(int min, int inclusiveMax); // [min, max]
int rollDice(int n, int s); // roll S sided dice N times

std::wstring capitalize(const std::wstring& string);

// Bresenham's line algorithm
std::vector<sf::Vector2i> plotPath(sf::Vector2i p1, sf::Vector2i p2);

template <typename T>
T length(const sf::Vector2<T>& vector);

template <typename T>
T lengthSquared(const sf::Vector2<T>& vector);

#include "Utility.inl"