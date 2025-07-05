#pragma once

#include <sol/sol.hpp>

class LuaBinder
{
public:
	static void CallBinders(sol::state& p_luaState);
};

