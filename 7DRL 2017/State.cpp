#include "State.hpp"
#include "StateStack.hpp"

State::State(StateStack& stack)
	: stack(stack)
{
}

void State::requestStackPush(ID id)
{
	stack.pushState(id);
}

void State::requestStackPop()
{
	stack.popState();
}

void State::requestStateClear()
{
	stack.clearStates();
}

Context& State::getContext()
{
	return stack.getContext();
}