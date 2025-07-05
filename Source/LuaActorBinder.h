#pragma once

#include <sol/sol.hpp>

class LuaActorBinder
{
public:
	static void BindActor(sol::state& p_luaState);
};

