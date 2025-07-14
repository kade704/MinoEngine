#pragma once

#include "Text.h"
#include "../Event.h"

namespace Widget
{
    class TextClickable : public Text
    {
    public:
        TextClickable(const std::string& p_content = "") :
            Text(p_content)
        {
        }

    protected:
        virtual void _Draw_Impl() override
        {
            bool useless = false;

            if (ImGui::Selectable((content + m_widgetID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
            {
                if (ImGui::IsMouseDoubleClicked(0))
                {
                    DoubleClickedEvent.Invoke();
                }
                else
                {
                    ClickedEvent.Invoke();
                }
            }
        }

    public:
        Event<> ClickedEvent;
        Event<> DoubleClickedEvent;
    };
}