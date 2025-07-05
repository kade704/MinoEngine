#include "CLight.h"
#include "../Actor.h"
#include "../Serializer.h"
#include "../GUIDrawer.h"

Component::CLight::CLight(Actor& p_owner) :
	AComponent(p_owner),
	m_data(p_owner.transform.GetFTransform(), {})
{
}

Component::CLight::~CLight()
{
}

const FVector3& Component::CLight::GetColor() const
{
	return m_data.color;
}

void Component::CLight::SetColor(const FVector3& p_color)
{
	m_data.color = p_color;
}

const Light& Component::CLight::GetData() const
{
	return m_data;
}

void Component::CLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Serializer::SerializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::SerializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void Component::CLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Serializer::DeserializeVec3(p_doc, p_node, "color", m_data.color);
	Serializer::DeserializeFloat(p_doc, p_node, "intensity", m_data.intensity);
}

void Component::CLight::OnInspector(WidgetContainer& p_root)
{
	GUIDrawer::DrawColor(p_root, "Color", reinterpret_cast<Color&>(m_data.color));
	GUIDrawer::DrawScalar<float>(p_root, "Intensity", m_data.intensity, 0.005f, GUIDrawer::_MIN_FLOAT, GUIDrawer::_MAX_FLOAT);
}
