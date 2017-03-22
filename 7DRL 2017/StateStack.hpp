#pragma once

// Credit: SFML Game Development Book
// https://github.com/SFML/SFML-Game-Development-Book

#include "State.hpp"

#include <vector>
#include <unordered_map>
#include <functional>

class StateStack
{
public:
	explicit StateStack(Context context);

	template <typename T>
	void registerState(const State::ID& id);

	void handleEvent(const sf::Event& event);
	void update(sf::Time dt);
	void draw();

	void pushState(const State::ID& id);
	void popState();
	void clearStates();

	bool isEmpty() const;

	Context& getContext();

private:
	enum Action
	{
		Push,
		Pop,
		Clear
	};

	struct PendingChange
	{
		Action action;
		State::ID id;
	};

private:
	void applyPendingChanges();

private:
	Context context;
	std::vector<State::Ptr> stack;
	std::vector<PendingChange> pendingList;
	std::unordered_map<State::ID, std::function<State::Ptr()>> factories;
};

template <typename T>
void StateStack::registerState(const State::ID& id)
{
	factories[id] = [this] ()
	{
		return std::make_unique<T>(*this);
	};
}