#include "APanel.h"

uint64_t Panel::APanel::__PANEL_ID_INCREMENT = 0;

Panel::APanel::APanel()
{
    m_panelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void Panel::APanel::Draw()
{
    _Draw_Impl();
}

const std::string& Panel::APanel::GetPanelID() const
{
    return m_panelID;
}
