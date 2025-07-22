#include "LuaActorBinder.h"
#include "../Actor.h"
#include "../Component/CTransform.h"

void LuaActorBinder::BindActor(sol::state& p_luaState)
{
	using namespace Component;

	p_luaState.new_usertype<Actor>("Actor",
		"GetName", &Actor::GetName,
		"SetName", &Actor::SetName,
		"GetParent", &Actor::GetParent,
		"SetParent", &Actor::SetParent,
		"GetTransform", &Actor::GetComponent<CTransform>
	);
}
