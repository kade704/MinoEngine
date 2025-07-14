#pragma once

#include "AComponent.h"
#include "../Math/FTransform.h"

#include <vector>

namespace Component
{
	class CTransform : public AComponent
	{
		COMPONENT_NAME("Transform")

	public:
		CTransform(Actor& owner);

		void SetParent(CTransform& parent);
		bool HasParent() const;
		void RemoveParent();

		void SetLocalPosition(const FVector3& pos);
		void SetLocalRotation(const FQuaternion& rot);
		void SetLocalScale(const FVector3& scale);

		const FVector3& GetLocalPosition() const;
		const FQuaternion& GetLocalRotation() const;
		const FVector3& GetLocalScale() const;

		const FVector3& GetWorldPosition() const;
		const FQuaternion& GetWorldRotation() const;
		const FVector3& GetWorldScale() const;

		const FMatrix4& GetWorldMatrix() const;

		FTransform& GetFTransform();

		virtual void OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;

		virtual void OnInspector(WidgetContainer& p_root) override;

	private:
		FTransform m_transform;
	};
}
