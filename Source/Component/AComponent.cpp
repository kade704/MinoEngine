#include "AComponent.h"

Component::AComponent::AComponent(Actor& owner) :
	owner(owner)
{
}

Component::AComponent::~AComponent()
{
}

Actor& Component::AComponent::GetOwner() const
{
	return owner;
}