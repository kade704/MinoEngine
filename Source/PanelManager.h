#pragma once

#include "Panel/APanel.h"
#include "Panel/PanelWindow.h"
#include "Canvas.h"

#include <unordered_map>
#include <memory>

class PanelManager
{
public:
	PanelManager(Canvas& p_canvas);

	template<typename T, typename... Args>
	void CreatePanel(const std::string& p_id, Args&&... p_args)
	{
		if constexpr (std::is_base_of<Panel::PanelWindow, T>::value)
		{
			m_panels.emplace(p_id, std::make_unique<T>(p_id, std::forward<Args>(p_args)...));
			T& instance = *static_cast<T*>(m_panels.at(p_id).get());
			//GetPanelAs<OvEditor::Panels::MenuBar>("Menu Bar").RegisterPanel(instance.name, instance);
		}
		else
		{
			m_panels.emplace(p_id, std::make_unique<T>(std::forward<Args>(p_args)...));
		}

		m_canvas.AddPanel(*m_panels.at(p_id));
	}

	template<typename T>
	T* GetPanelAs(const std::string& p_id)
	{
		return static_cast<T*>(m_panels[p_id].get());
	}

private:
	std::unordered_map<std::string, std::unique_ptr<Panel::APanel>> m_panels;
	Canvas& m_canvas;
};