#pragma once

#include <sol/sol.hpp>

class LuaGlobalBinder
{
public:
	static void BindGlobal(sol::state& p_luaState);
};

