#pragma once

#include <string>

#include "../IInspectorItem.h"

#define COMPONENT_NAME(name) \
public: \
	static std::string GetNameStatic() { return name; } \
	virtual std::string GetName() { return GetNameStatic(); } \

class Actor;

namespace Component
{
	class AComponent : public IInspectorItem
	{
	public:
		AComponent(Actor& owner);
		virtual ~AComponent();

		virtual void OnStart() {}
		virtual void OnUpdate(float deltaTime) {}

		virtual std::string GetName() = 0;

		Actor& GetOwner() const;

	public:
		Actor& owner;
	};
}