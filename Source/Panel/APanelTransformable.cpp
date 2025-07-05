#include "APanelTransformable.h"
#include "../Converter.h"

Panel::APanelTransformable::APanelTransformable
(
    const FVector2& p_defaultPosition,
    const FVector2& p_defaultSize,
    EHorizontalAlignment p_defaultHorizontalAlignment,
    EVerticalAlignment p_defaultVerticalAlignment
) :
    m_defaultPosition(p_defaultPosition),
    m_defaultSize(p_defaultSize),
    m_defaultHorizontalAlignment(p_defaultHorizontalAlignment),
    m_defaultVerticalAlignment(p_defaultVerticalAlignment)
{
}

void Panel::APanelTransformable::SetPosition(const FVector2& p_position)
{
    m_position = p_position;
    m_positionChanged = true;
}

void Panel::APanelTransformable::SetSize(const FVector2& p_size)
{
    m_size = p_size;
    m_sizeChanged = true;
}

void Panel::APanelTransformable::SetAlignment(EHorizontalAlignment p_horizontalAlignment, EVerticalAlignment p_verticalAligment)
{
    m_horizontalAlignment = p_horizontalAlignment;
    m_verticalAlignment = p_verticalAligment;
    m_alignmentChanged = true;
}

const FVector2& Panel::APanelTransformable::GetPosition() const
{
    return m_position;
}

const FVector2& Panel::APanelTransformable::GetSize() const
{
    return m_size;
}

void Panel::APanelTransformable::Update()
{
    if (!m_firstFrame)
    {
        if (!autoSize)
            UpdateSize();
        CopyImGuiSize();

        UpdatePosition();
        CopyImGuiPosition();
    }

    m_firstFrame = false;
}

FVector2 Panel::APanelTransformable::CalculatePositionAlignmentOffset(bool p_default)
{
    FVector2 result(0.0f, 0.0f);

    switch (p_default ? m_defaultHorizontalAlignment : m_horizontalAlignment)
    {
    case EHorizontalAlignment::CENTER:
        result.x -= m_size.x / 2.0f;
        break;
    case EHorizontalAlignment::RIGHT:
        result.x -= m_size.x;
        break;
    }

    switch (p_default ? m_defaultVerticalAlignment : m_verticalAlignment)
    {
    case EVerticalAlignment::MIDDLE:
        result.y -= m_size.y / 2.0f;
        break;
    case EVerticalAlignment::BOTTOM:
        result.y -= m_size.y;
        break;
    }

    return result;
}

void Panel::APanelTransformable::UpdatePosition()
{
    if (m_defaultPosition.x != -1.f && m_defaultPosition.y != 1.f)
    {
        FVector2 offsettedDefaultPos = m_defaultPosition + CalculatePositionAlignmentOffset(true);
        ImGui::SetWindowPos(Converter::ToImVec2(offsettedDefaultPos));
    }

    if (m_positionChanged || m_alignmentChanged)
    {
        FVector2 offset = CalculatePositionAlignmentOffset(false);
        FVector2 offsettedPos(m_position.x + offset.x, m_position.y + offset.y);
        ImGui::SetWindowPos(Converter::ToImVec2(offsettedPos));
        m_positionChanged = false;
        m_alignmentChanged = false;
    }
}

void Panel::APanelTransformable::UpdateSize()
{
    if (m_sizeChanged)
    {
        ImGui::SetWindowSize(Converter::ToImVec2(m_size), ImGuiCond_Always);
        m_sizeChanged = false;
    }
}

void Panel::APanelTransformable::CopyImGuiPosition()
{
    m_position = Converter::ToFVector2(ImGui::GetWindowPos());
}

void Panel::APanelTransformable::CopyImGuiSize()
{
    m_size = Converter::ToFVector2(ImGui::GetWindowSize());
}
