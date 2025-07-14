#pragma once

#include <string>

#include "IPlugin.h"
#include "Event.h"

template<typename T>
class DDSource : public IPlugin
{
public:
	DDSource
	(
		const std::string& p_identifier,
		const std::string& p_tooltip,
		T p_data
	) : identifier(p_identifier), tooltip(p_tooltip), data(p_data)
	{}

	virtual void Execute() override
	{
		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

		if (!hasTooltip)
			src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;

		if (ImGui::BeginDragDropSource(src_flags))
		{
			if (!m_isDragged)
				DragStartEvent.Invoke();

			m_isDragged = true;

			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				ImGui::Text(tooltip.c_str());
			ImGui::SetDragDropPayload(identifier.c_str(), &data, sizeof(data));
			ImGui::EndDragDropSource();
		}
		else
		{
			if (m_isDragged)
				DragStopEvent.Invoke();

			m_isDragged = false;
		}
	}

	bool IsDragged() const
	{
		return m_isDragged;
	}

public:
	std::string identifier;
	std::string tooltip;
	T data;
	Event<> DragStartEvent;
	Event<> DragStopEvent;

	bool hasTooltip = true;

private:
	bool m_isDragged;
};