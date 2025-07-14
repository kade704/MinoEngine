#pragma once

#include <sol/sol.hpp>

class LuaMathBinder
{
public:
	static void BindMath(sol::state& p_luaState);
};

