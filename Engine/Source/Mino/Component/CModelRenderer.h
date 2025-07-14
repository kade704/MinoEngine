#pragma once

#include "AComponent.h"
#include "../Resource/Model.h"
#include "../Event.h"

namespace Component
{
	class CModelRenderer : public AComponent
	{
		COMPONENT_NAME("Model Renderer")

	public:
		CModelRenderer(Actor& p_owner);

		void SetModel(Model* p_model);
		Model* GetModel() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

	private:
		Model* m_model = nullptr;
		Event<> m_modelChangedEvent;
	};
}
