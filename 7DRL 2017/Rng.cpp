#include "Rng.hpp"

#include <iostream>

Rng::Rng(unsigned int seed)
	: seed(seed)
	, mt(seed)
{
	std::cout << "Random Seed = " << seed << std::endl;
}

void Rng::setSeed(unsigned int seed)
{
	this->seed = seed;
	mt.seed(seed);

	std::cout << "Random Seed = " << seed << std::endl;
}

unsigned int Rng::getSeed() const
{
	return seed;
}

int Rng::getInt(int exclusiveMax)
{
	std::uniform_int_distribution<> dist(0, exclusiveMax - 1);

	return dist(mt);
}

int Rng::getInt(int min, int inclusiveMax)
{
	std::uniform_int_distribution<> dist(min, inclusiveMax);

	return dist(mt);
}

int Rng::rollDice(int n, int s)
{
	int value = 0;

	for (int i = 0; i < n; ++i)
		value += getInt(1, s);

	return value;
}