#pragma once

#include "DataWidget.h"
#include "../WidgetContainer.h"
#include "../Event.h"

#include <imgui.h>

namespace Widget
{
	class TreeNode : public DataWidget<std::string>, public WidgetContainer
	{
	public:
		TreeNode(const std::string& p_name = "", bool p_arrowClickToOpen = false) :
			DataWidget(name),
			name(p_name),
			m_arrowClickToOpen(p_arrowClickToOpen)
		{
			m_autoExecutePlugins = false;
		}

		void Open()
		{
			m_shouldOpen = true;
			m_shouldClose = false;
		}

		void Close()
		{
			m_shouldClose = true;
			m_shouldOpen = false;
		}

		bool IsOpened() const
		{
			return m_opened;
		}

	protected:
		virtual void _Draw_Impl() override
		{
			bool prevOpened = m_opened;

			if (m_shouldOpen)
			{
				ImGui::SetNextItemOpen(true);
				m_shouldOpen = false;
			}
			else if (m_shouldClose)
			{
				ImGui::SetNextItemOpen(false);
				m_shouldClose = false;
			}

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			if (m_arrowClickToOpen) flags |= ImGuiTreeNodeFlags_OpenOnArrow;
			if (selected)			flags |= ImGuiTreeNodeFlags_Selected;
			if (leaf)				flags |= ImGuiTreeNodeFlags_Leaf;

			bool opened = ImGui::TreeNodeEx((name + m_widgetID).c_str(), flags);

			if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
			{
				ClickedEvent.Invoke();

				if (ImGui::IsMouseDoubleClicked(0))
				{
					DoubleClickedEvent.Invoke();
				}
			}

			if (opened)
			{
				if (!prevOpened)
					OpenedEvent.Invoke();

				m_opened = true;

				ExecutePlugins();

				DrawWidgets();

				ImGui::TreePop();
			}
			else
			{
				if (prevOpened)
					ClosedEvent.Invoke();

				m_opened = false;

				ExecutePlugins();
			}
		}

	public:
		std::string name;
		bool selected = false;
		bool leaf = false;

		Event<> ClickedEvent;
		Event<> DoubleClickedEvent;
		Event<> OpenedEvent;
		Event<> ClosedEvent;

	private:
		bool m_arrowClickToOpen = false;
		bool m_shouldOpen = false;
		bool m_shouldClose = false;
		bool m_opened = false;
	};
}
