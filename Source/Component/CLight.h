#pragma once

#include "AComponent.h"
#include "../Light.h"

namespace Component
{
	class CLight : public AComponent
	{
	public:
		CLight(Actor& p_owner);
		~CLight();

		const FVector3& GetColor() const;
		void SetColor(const FVector3& p_color);

		const Light& GetData() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

	protected:
		Light m_data;
	};
}
