#pragma once

#include "AComponent.h"
#include "../Camera.h"

namespace Component
{
	class CCamera : public AComponent
	{
		COMPONENT_NAME("Camera")

	public:
		CCamera(Actor& p_owner);
		~CCamera();

		void SetFov(float p_value);
		void SetNear(float p_value);
		void SetFar(float p_value);
		void SetClearColor(const FVector3& p_clearColor);

		float GetFov() const;
		float GetNear() const;
		float GetFar() const;
		const FVector3& GetClearColor() const;

		Camera& GetCamera();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

	private:
		Camera m_camera;
	};
}
