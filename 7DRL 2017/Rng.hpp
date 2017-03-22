#pragma once

#include <random>

// Random number generator
class Rng
{
public:
	explicit Rng(unsigned int seed = std::random_device()());

	void setSeed(unsigned int seed);
	unsigned int getSeed() const;

	int getInt(int exclusiveMax); // [0, max)
	int getInt(int min, int inclusiveMax); // [min, max]
	int rollDice(int n, int s); // roll S sided dice N times

	// HACK: used for enum class
	template <typename T>
	T getInt(T type);

private:
	unsigned int seed;
	std::mt19937 mt;
};

template <typename T>
T Rng::getInt(T type)
{
	return static_cast<T>(getInt(static_cast<int>(type)));
}