#include "Menubar.h"
#include "../EditorAction.h"

Panel::Menubar::Menubar()
{
	CreateFileMenu();
}

void Panel::Menubar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<Widget::MenuList>("File");
	fileMenu.CreateWidget<Widget::MenuItem>("New Scene", "CTRL + N").ClickedEvent += EDITOR_BIND(LoadEmptyScene);
	fileMenu.CreateWidget<Widget::MenuItem>("Save Scene", "CTRL + S").ClickedEvent += EDITOR_BIND(SaveAs);
}
