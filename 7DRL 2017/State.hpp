#pragma once

#include "Context.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <memory> // unique_ptr

namespace sf
{
	class Event;
}

class StateStack;

class State
{
public:
	using ID = std::string;
	using Ptr = std::unique_ptr<State>;

public:
	explicit State(StateStack& stack);
	virtual ~State() = default;

	virtual bool handleEvent(const sf::Event& event) = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual void draw() = 0;

protected:
	void requestStackPush(ID id);
	void requestStackPop();
	void requestStateClear();

	Context& getContext();

private:
	StateStack& stack;
};