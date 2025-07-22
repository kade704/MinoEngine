#include "ScriptInterpreter.h"
#include "LuaBinder.h"
#include "../Logger.h"

ScriptInterpreter::ScriptInterpreter()
{
	CreateLuaContextAndBindGlobals();

	Component::Behaviour::CreatedEvent += std::bind(&ScriptInterpreter::Consider, this, std::placeholders::_1);
	Component::Behaviour::DestroyedEvent += std::bind(&ScriptInterpreter::Unconsider, this, std::placeholders::_1);
}

ScriptInterpreter::~ScriptInterpreter()
{
	DestroyLuaContext();
}

void ScriptInterpreter::CreateLuaContextAndBindGlobals()
{
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);
		LuaBinder::CallBinders(*m_luaState);
		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](Component::Behaviour* behaviour)
			{
				if (!behaviour->RegisterToLuaContext(*m_luaState, "Contents/"))
				m_isOk = false;
			});

		if (!m_isOk)
			MINO_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
	}
}

void ScriptInterpreter::DestroyLuaContext()
{
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](Component::Behaviour* behaviour)
			{
				behaviour->UnregisterFromLuaContext();
			});

		m_luaState.reset();
		m_isOk = false;
	}
}

void ScriptInterpreter::Consider(Component::Behaviour* p_toConsider)
{
	if (m_luaState)
	{
		m_behaviours.push_back(p_toConsider);

		if (!p_toConsider->RegisterToLuaContext(*m_luaState, "Contents/"))
			m_isOk = false;
	}
}

void ScriptInterpreter::Unconsider(Component::Behaviour* p_toUnconsider)
{
	if (m_luaState)
		p_toUnconsider->UnregisterFromLuaContext();

	m_behaviours.erase(std::remove_if(m_behaviours.begin(), m_behaviours.end(), [p_toUnconsider](Component::Behaviour* behaviour)
		{
			return p_toUnconsider == behaviour;
		}));

	RefreshAll();
}

void ScriptInterpreter::RefreshAll()
{
	DestroyLuaContext();
	CreateLuaContextAndBindGlobals();
}

bool ScriptInterpreter::IsOk() const
{
	return m_isOk;
}