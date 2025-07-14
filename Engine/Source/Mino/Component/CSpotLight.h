#pragma once

#include "CLight.h"

namespace Component
{
	class CSpotLight : public CLight
	{
		COMPONENT_NAME("Spot Light")

	public:
		CSpotLight(Actor& p_owner);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;
	};
}
