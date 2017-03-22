#pragma once

#include "Actor.hpp"

class ActorEnemy : public Actor
{
public:
	explicit ActorEnemy(ActorData& data);

	ActorType getActorType() const override;

private:
};