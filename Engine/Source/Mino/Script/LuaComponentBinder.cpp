#include "LuaComponentBinder.h"
#include "../Component/CTransform.h"
#include "../Actor.h"

void LuaComponentBinder::BindComponent(sol::state& p_luaState)
{
	using namespace Component;

	p_luaState.new_usertype<AComponent>("Component",
		"GetOwner", [](AComponent& p_component) -> Actor& { return p_component.owner; }
	);

	p_luaState.new_usertype<CTransform>("Transform",
		sol::base_classes, sol::bases<AComponent>(),
		/* Methods */
		"SetPosition", &CTransform::SetLocalPosition,
		"SetRotation", &CTransform::SetLocalRotation,
		"SetScale", &CTransform::SetLocalScale,
		"SetLocalPosition", &CTransform::SetLocalPosition,
		"SetLocalRotation", &CTransform::SetLocalRotation,
		"SetLocalScale", &CTransform::SetLocalScale,
		"GetPosition", [](CTransform& p_this) -> FVector3 { return p_this.GetWorldPosition(); },
		"GetRotation", [](CTransform& p_this) -> FQuaternion { return p_this.GetWorldRotation(); },
		"GetScale", [](CTransform& p_this) -> FVector3 { return p_this.GetWorldScale(); },
		"GetLocalPosition", [](CTransform& p_this) -> FVector3 { return p_this.GetLocalPosition(); },
		"GetLocalRotation", [](CTransform& p_this) -> FQuaternion { return p_this.GetLocalRotation(); },
		"GetLocalScale", [](CTransform& p_this) -> FVector3 { return p_this.GetLocalScale(); },
		"GetWorldPosition", [](CTransform& p_this) -> FVector3 { return p_this.GetWorldPosition(); },
		"GetWorldRotation", [](CTransform& p_this) -> FQuaternion { return p_this.GetWorldRotation(); },
		"GetWorldScale", [](CTransform& p_this) -> FVector3 { return p_this.GetWorldScale(); }
		);
}
