#pragma once

#include <Mino/Panel/PanelMenubar.h>
#include <Mino/Widget/MenuList.h>
#include <Mino/Widget/MenuItem.h>

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
