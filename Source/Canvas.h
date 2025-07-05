#pragma once

#include "IDrawable.h"
#include "Panel/APanel.h"

class Canvas : public IDrawable
{
public:
	void AddPanel(Panel::APanel& p_panel);
	void RemovePanel(Panel::APanel& p_panel);

	void Draw() override;

private:
	std::vector<std::reference_wrapper<Panel::APanel>> m_panels;
};

