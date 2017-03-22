#include "StateStack.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

StateStack::StateStack(Context context)
	: context(context)
{
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto it = stack.rbegin(); it != stack.rend(); ++it)
	{
		if (!(*it)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
	for (auto it = stack.rbegin(); it != stack.rend(); ++it)
	{
		if (!(*it)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto& state : stack)
		state->draw();
}

void StateStack::pushState(const State::ID& id)
{
	pendingList.push_back({ Push, id });
}

void StateStack::popState()
{
	pendingList.push_back({ Pop, "" });
}

void StateStack::clearStates()
{
	pendingList.push_back({ Clear, "" });
}

bool StateStack::isEmpty() const
{
	return stack.empty();
}

Context& StateStack::getContext()
{
	return context;
}

void StateStack::applyPendingChanges()
{
	for (auto change : pendingList)
	{
		switch (change.action)
		{
		case Push:
			stack.emplace_back(factories[change.id]());
			break;

		case Pop:
			stack.pop_back();
			break;

		case Clear:
			stack.clear();
			break;
		}
	}

	pendingList.clear();
}