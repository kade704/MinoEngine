#include "CMaterialRenderer.h"
#include "../Manager/MaterialManager.h"
#include "../Serializer.h"
#include "../imgui/imgui.h"
#include "../Actor.h"
#include "../Component/CModelRenderer.h"
#include "../GUIDrawer.h"
#include "../Widget/TextColored.h"
#include "../Widget/Group.h"
#include "../Widget/SmallButton.h"
#include "../DDTarget.h"
#include "../PathParser.h"
#include "../ServiceLocator.h"
#include "../Manager/MaterialManager.h"

Component::CMaterialRenderer::CMaterialRenderer(Actor& owner) :
    AComponent(owner)
{
    m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; ++i)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

Component::CMaterialRenderer::~CMaterialRenderer()
{
}

const Component::CMaterialRenderer::MaterialList& Component::CMaterialRenderer::GetMaterials() const
{
    return m_materials;
}

const FMatrix4 &Component::CMaterialRenderer::GetUserMatrix() const
{
    return m_userMatrix;
}

void Component::CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsNode = p_doc.NewElement("materials");
	p_node->InsertEndChild(materialsNode);

	auto modelRenderer = owner.GetComponent<CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		Serializer::SerializeMaterial(p_doc, materialsNode, "material", m_materials[i]);
	}
}

void Component::CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsRoot = p_node->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			m_materials[materialIndex] = ServiceLocator::Get<MaterialManager>().GetResource(currentMaterial->GetText());

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

void Component::CMaterialRenderer::OnInspector(WidgetContainer& p_root)
{
	for (uint8_t i = 0; i < m_materials.size(); ++i)
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);
		
	UpdateMaterialList();
}

void Component::CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = owner.GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			m_materialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; ++i)
			m_materialNames[i] = "";
	}

	for (uint8_t i = 0; i < m_materialFields.size(); ++i)
	{
		if (m_materialFields[i][0])
		{
			bool enabled = !m_materialNames[i].empty();
			m_materialFields[i][0]->enabled = enabled;
			m_materialFields[i][1]->enabled = enabled;
			m_materialFields[i][2]->enabled = enabled;
			reinterpret_cast<Widget::Text*>(m_materialFields[i][0])->content = m_materialNames[i];
		}
	}
}

std::array<Widget::AWidget*, 3> Component::CMaterialRenderer::CustomMaterialDrawer(WidgetContainer& p_root, const std::string& p_name, Material*& p_data)
{
	std::array<Widget::AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<Widget::TextColored>(p_name, GUIDrawer::TitleColor);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto& rightSide = p_root.CreateWidget<Widget::Group>();
	auto& widget = rightSide.CreateWidget<Widget::Text>(displayedText);

	widgets[1] = &widget;

	widget.AddPlugin<DDTarget<std::pair<std::string, Widget::Group*>>>("File").DataReceivedEvent += [&widget, &p_data](auto p_receivedData)
	{
		if (PathParser::GetFileType(p_receivedData.first) == PathParser::EFileType::MATERIAL)
		{
			if (auto resource = MINO_SERVICE(MaterialManager).GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.content = p_receivedData.first;
			}
		}
	};

	widget.lineBreak = false;

	auto& resetButton = rightSide.CreateWidget<Widget::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = GUIDrawer::ClearButtonColor;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}
