#include "CSpotLight.h"
#include "../Serializer.h"
#include "../GUIDrawer.h"

Component::CSpotLight::CSpotLight(Actor& p_owner) :
    CLight(p_owner)
{
    m_data.type = static_cast<float>(Light::Type::SPOT);
}

void Component::CSpotLight::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnSerialize(p_doc, p_node);

	Serializer::SerializeFloat(p_doc, p_node, "constant", m_data.constant);
	Serializer::SerializeFloat(p_doc, p_node, "linear", m_data.linear);
	Serializer::SerializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
	Serializer::SerializeFloat(p_doc, p_node, "cutoff", m_data.cutoff);
	Serializer::SerializeFloat(p_doc, p_node, "outercutoff", m_data.outerCutoff);
}

void Component::CSpotLight::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CLight::OnDeserialize(p_doc, p_node);

	Serializer::DeserializeFloat(p_doc, p_node, "constant", m_data.constant);
	Serializer::DeserializeFloat(p_doc, p_node, "linear", m_data.linear);
	Serializer::DeserializeFloat(p_doc, p_node, "quadratic", m_data.quadratic);
	Serializer::DeserializeFloat(p_doc, p_node, "cutoff", m_data.cutoff);
	Serializer::DeserializeFloat(p_doc, p_node, "outercutoff", m_data.outerCutoff);
}

void Component::CSpotLight::OnInspector(WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);

	GUIDrawer::DrawScalar<float>(p_root, "Cut-Off", m_data.cutoff, 0.1f, 0.f, 180.f);
	GUIDrawer::DrawScalar<float>(p_root, "Outer Cut-Off", m_data.outerCutoff, 0.1f, 0.f, 180.f);

	GUIDrawer::DrawScalar<float>(p_root, "Constant", m_data.constant, 0.005f, 0.f);
	GUIDrawer::DrawScalar<float>(p_root, "Linear", m_data.linear, 0.005f, 0.f);
	GUIDrawer::DrawScalar<float>(p_root, "Quadratic", m_data.quadratic, 0.005f, 0.f);
}
