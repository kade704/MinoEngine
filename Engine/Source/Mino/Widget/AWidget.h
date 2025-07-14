#pragma once

#include "../IDrawable.h"
#include "../Pluginable.h"
#include "../DataDispatcher.h"
#include <string>

class WidgetContainer;

namespace Widget
{
	class AWidget : public IDrawable, public Pluginable
	{
	public:
		AWidget();

		virtual void Draw() override;

		void LinkTo(const AWidget& widget);

		void Destroy();

		bool IsDestroyed() const;

		void SetParent(WidgetContainer* parent);

		bool HasParent() const;

		WidgetContainer* GetParent();

	protected:
		virtual void _Draw_Impl() = 0;

	public:
		bool enabled = true;
		bool lineBreak = true;

	protected:
		WidgetContainer* m_parent = nullptr;
		std::string m_widgetID = "?";
		bool m_autoExecutePlugins = true;

	private:
		static uint64_t __WIDGET_ID_INCREMENT;
		bool m_destoryed = false;
	};
}
