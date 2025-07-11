#pragma once

#include "Widget/AWidget.h"
#include <vector>

enum class EMemoryMode
{
	INTERNAL_MANAGMENT,
	EXTERNAL_MANAGMENT
};

class WidgetContainer
{
public:
	WidgetContainer() = default;
	virtual ~WidgetContainer();

	void RemoveWidget(Widget::AWidget& widget);
	void RemoveAllWidgets();

	void ConsiderWidget(Widget::AWidget& p_widget, bool p_manageMemory = true);
	void UnconsiderWidget(Widget::AWidget& p_widget);

	void DrawWidgets();

	void CollectGarbages();

	template <typename T, typename ... Args>
	T& CreateWidget(Args&&... p_args)
	{
		m_widgets.emplace_back(new T(p_args...), EMemoryMode::INTERNAL_MANAGMENT);
		T& instance = *reinterpret_cast<T*>(m_widgets.back().first);
		instance.SetParent(this);
		return instance;
	}

	void ReverseDrawOrder(bool p_reversed = true);

	std::vector<std::pair<Widget::AWidget*, EMemoryMode>>& GetWidgets();

protected:
	std::vector<std::pair<Widget::AWidget*, EMemoryMode>> m_widgets;
	bool m_reversedDrawOrder = false;
};

