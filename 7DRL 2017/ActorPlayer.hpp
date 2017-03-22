#pragma once

#include "Actor.hpp"

class ActorPlayer : public Actor
{
public:
	explicit ActorPlayer(ActorData& data);

	ActorType getActorType() const override;
	
	bool isMarkedForRemoval() const override;

private:
};