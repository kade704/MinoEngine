#pragma once

#include "APanel.h"
#include "../Math/FVector2.h"

namespace Panel
{
	enum class EHorizontalAlignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

	enum class EVerticalAlignment
	{
		TOP,
		MIDDLE,
		BOTTOM
	};

	class APanelTransformable : public APanel
	{
	public:
		APanelTransformable
		(
			const FVector2& p_defaultPosition = FVector2(-1.f, -1.f),
			const FVector2& p_defaultSize = FVector2(-1.f, -1.f),
			EHorizontalAlignment p_defaultHorizontalAlignment = EHorizontalAlignment::LEFT,
			EVerticalAlignment p_defaultVerticalAlignment = EVerticalAlignment::TOP
		);

		void SetPosition(const FVector2& p_position);
		void SetSize(const FVector2& p_size);
		void SetAlignment(EHorizontalAlignment p_horizontalAlignment, EVerticalAlignment p_verticalAligment);

		const FVector2& GetPosition() const;
		const FVector2& GetSize() const;

	protected:
		void Update();
		virtual void _Draw_Impl() = 0;

	private:
		FVector2 CalculatePositionAlignmentOffset(bool p_default = false);

		void UpdatePosition();
		void UpdateSize();
		void CopyImGuiPosition();
		void CopyImGuiSize();

	public:
		bool autoSize = true;

	protected:
		FVector2 m_defaultPosition;
		FVector2 m_defaultSize;
		EHorizontalAlignment m_defaultHorizontalAlignment;
		EVerticalAlignment m_defaultVerticalAlignment;

		FVector2 m_position = FVector2::Zero;
		FVector2 m_size = FVector2::Zero;

		bool m_positionChanged = false;
		bool m_sizeChanged = false;

		EHorizontalAlignment m_horizontalAlignment = EHorizontalAlignment::LEFT;
		EVerticalAlignment m_verticalAlignment = EVerticalAlignment::TOP;

		bool m_alignmentChanged = false;
		bool m_firstFrame = true;
	};
}
