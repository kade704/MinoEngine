#include <Component/Behaviour.h>
#include <Actor.h>
#include <Widget/TextColored.h>
#include <Widget/ButtonImage.h>
#include <EditorAction.h>
#include <Panel/Inspector.h>

Event<Component::Behaviour*> Component::Behaviour::CreatedEvent;
Event<Component::Behaviour*> Component::Behaviour::DestroyedEvent;

Component::Behaviour::Behaviour(Actor& p_owner, const std::string& p_name) :
	name(p_name),
	AComponent(p_owner)
{
	CreatedEvent.Invoke(this);
}

Component::Behaviour::~Behaviour()
{
	DestroyedEvent.Invoke(this);
}

bool Component::Behaviour::RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder)
{
	auto result = p_luaState.safe_script_file(p_scriptFolder + name + ".lua", &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result;
		MINO_ERROR(err.what());
		return false;
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			m_object = result[0];
			m_object["owner"] = &owner;
			return true;
		}
		else
		{
			MINO_ERROR("'" + name + ".lua' missing return expression");
			return false;
		}
	}
}

void Component::Behaviour::UnregisterFromLuaContext()
{
	m_object = sol::nil;
}

void Component::Behaviour::OnStart()
{
	LuaCall("OnStart");
}

void Component::Behaviour::OnUpdate(float p_deltaTime)
{
	LuaCall("OnUpdate", p_deltaTime);
}

void Component::Behaviour::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void Component::Behaviour::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void Component::Behaviour::OnInspector(WidgetContainer& p_root)
{
	if (m_object.valid())
	{
		p_root.CreateWidget<Widget::TextColored>("Ready", Color::Green);
		p_root.CreateWidget<Widget::TextColored>("Your script gets interpreted by the engine with success", Color::White);
	}
	else
	{
		p_root.CreateWidget<Widget::TextColored>("Compilation failed!", Color::Red);
		p_root.CreateWidget<Widget::TextColored>("Check the console for more information", Color::White);
	}
}
