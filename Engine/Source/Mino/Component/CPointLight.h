#pragma once

#include "CLight.h"

namespace Component
{
	class CPointLight : public CLight
	{
		COMPONENT_NAME("Point Light")

	public:
		CPointLight(Actor& p_owner);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;
	};
}
