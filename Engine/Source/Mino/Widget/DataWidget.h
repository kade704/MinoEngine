#pragma once

#include "AWidget.h"

namespace Widget
{

	template <typename T>
	class DataWidget : public AWidget
	{
	public:
		DataWidget(T& dataHolder) : m_data(dataHolder) {}

		virtual void Draw() override
		{
			TRY_GATHER(T, m_data);
			AWidget::Draw();
			TRY_PROVIDE(T, m_data);
		}

		void NotifyChange()
		{
			TRY_NOTIFY_CHANGE(T);
		}

	private:
		T& m_data;
	};
}