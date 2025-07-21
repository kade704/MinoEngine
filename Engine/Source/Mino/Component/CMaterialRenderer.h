#pragma once

#include <array>

#include "AComponent.h"
#include "../Resource/Material.h"

#define MAX_MATERIAL_COUNT 255

namespace Component
{
	class CMaterialRenderer : public AComponent
	{
		COMPONENT_NAME("Material Renderer")

	public:
		using MaterialList = std::array<Material*, MAX_MATERIAL_COUNT>;
		using MaterialField = std::array<std::array<Widget::AWidget*, 3>, MAX_MATERIAL_COUNT>;

		CMaterialRenderer(Actor& owner);
		~CMaterialRenderer();

		const FMatrix4& GetUserMatrix() const;
		const MaterialList& GetMaterials() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

		void UpdateMaterialList();

		std::array<Widget::AWidget*, 3> CustomMaterialDrawer(WidgetContainer& p_root, const std::string& p_name, Material*& p_data);

	private:
		MaterialList m_materials;
		MaterialField m_materialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> m_materialNames;
		FMatrix4 m_userMatrix;
	};
}
