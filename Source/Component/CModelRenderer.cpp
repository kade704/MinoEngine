#include "CModelRenderer.h"
#include "CMaterialRenderer.h"
#include "../Serializer.h"
#include "../GUIDrawer.h"
#include "../Actor.h"

Component::CModelRenderer::CModelRenderer(Actor& p_owner) :
	AComponent(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (auto materialRenderer = owner.GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

void Component::CModelRenderer::SetModel(Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

Model* Component::CModelRenderer::GetModel() const
{
	return m_model;
}

void Component::CModelRenderer::OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
	Serializer::SerializeModel(doc, node, "path", m_model);
}

void Component::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
	Serializer::DeserializeModel(doc, node, "path", m_model);
}

void Component::CModelRenderer::OnInspector(WidgetContainer& p_root)
{
	GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);
}
