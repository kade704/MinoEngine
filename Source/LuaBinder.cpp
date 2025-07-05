#include <LuaBinder.h>
#include <LuaGlobalBinder.h>
#include <LuaMathBinder.h>
#include <LuaActorBinder.h>
#include <LuaComponentBinder.h>

void LuaBinder::CallBinders(sol::state& p_luaState)
{
	auto& L = p_luaState;

	LuaGlobalBinder::BindGlobal(L);
	LuaMathBinder::BindMath(L);
	LuaActorBinder::BindActor(L);
	LuaComponentBinder::BindComponent(L);
}
