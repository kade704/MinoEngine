#pragma once

#include "CLight.h"

namespace Component
{
	class CDirectionalLight : public CLight
	{
		COMPONENT_NAME("Directional Light")

	public:
		CDirectionalLight(Actor& p_owner);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;
	};
}
