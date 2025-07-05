#pragma once

#include "PanelMenubar.h"
#include "../Widget/MenuList.h"
#include "../Widget/MenuItem.h"

namespace Panel
{
	class Menubar : public PanelMenubar
	{
	public:
		Menubar();

	private:
		void CreateFileMenu();

	private:
		Widget::MenuList* m_windowMenu = nullptr;
	};
}
