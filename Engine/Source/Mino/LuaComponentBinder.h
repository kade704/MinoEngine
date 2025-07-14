#pragma once

#include <sol/sol.hpp>

class LuaComponentBinder
{
public:
	static void BindComponent(sol::state& p_luaState);
};

