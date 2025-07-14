#include "AWidget.h"

#include <imgui.h>

uint64_t Widget::AWidget::__WIDGET_ID_INCREMENT = 0;

Widget::AWidget::AWidget()
{
    m_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void Widget::AWidget::Draw()
{
	if (enabled)
	{
		_Draw_Impl();

        if (m_autoExecutePlugins)
            ExecutePlugins();

        if (!lineBreak)
            ImGui::SameLine();
	}
}

void Widget::AWidget::LinkTo(const AWidget& widget)
{
    m_widgetID = widget.m_widgetID;
}

void Widget::AWidget::Destroy()
{
    m_destoryed = true;
}

bool Widget::AWidget::IsDestroyed() const
{
    return m_destoryed;
}

void Widget::AWidget::SetParent(WidgetContainer* parent)
{
    m_parent = parent;
}

bool Widget::AWidget::HasParent() const
{
    return m_parent;
}

WidgetContainer* Widget::AWidget::GetParent()
{
    return m_parent;
}
