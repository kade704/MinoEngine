#pragma once

#include <sol/sol.hpp>

#include "AComponent.h"
#include "../Logger.h"

class Actor;

namespace Component
{
	class Behaviour : public AComponent
	{
		COMPONENT_NAME("Behaviour")

	public:
		Behaviour(Actor& p_owner, const std::string& p_name);
		~Behaviour();

		bool RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder);
		void UnregisterFromLuaContext();

		void OnStart();
		void OnUpdate(float p_deltaTime);

		template<typename ...Args>
		void LuaCall(const std::string& p_functionName, Args&& ...p_args)
		{
			if (m_object.valid())
			{
				if (m_object[p_functionName].valid())
				{
					sol::protected_function pfr = m_object[p_functionName];
					auto pfrResult = pfr.call(m_object, std::forward<Args>(p_args)...);
					if (!pfrResult.valid())
					{
						sol::error err = pfrResult;
						MINO_ERROR(err.what());
					}
				}
			}
		}

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

	public:
		static Event<Behaviour*> CreatedEvent;
		static Event<Behaviour*> DestroyedEvent;

		const std::string name;

	private:
		sol::table m_object = sol::nil;
	};
}
