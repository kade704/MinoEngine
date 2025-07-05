#pragma once

#include <sol/sol.hpp>

#include <Component/Behaviour.h>

class ScriptInterpreter
{
public:
	ScriptInterpreter();
	~ScriptInterpreter();

	void CreateLuaContextAndBindGlobals();
	void DestroyLuaContext();

	void Consider(Component::Behaviour* p_toConsider);
	void Unconsider(Component::Behaviour* p_toUnconsider);

	void RefreshAll();

	bool IsOk() const;

private:
	std::unique_ptr<sol::state> m_luaState;
	std::vector<Component::Behaviour*> m_behaviours;
	bool m_isOk;
};