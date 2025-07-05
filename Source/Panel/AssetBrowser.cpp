#include "AssetBrowser.h"
#include "../PathParser.h"
#include "../Widget/TextClickable.h"
#include "../EditorAction.h"
#include "../DDSource.h"
#include "../ContextualMenu.h"
#include "../Widget/MenuList.h"
#include "../Widget/MenuItem.h"

class BrowserItemContextualMenu : public ContextualMenu
{
public:
	BrowserItemContextualMenu(const std::string p_filePath) : filePath(p_filePath) {}

	virtual void CreateList()
	{
		//auto& renameMenu = CreateWidget<MenuList>("Rename to...");
		auto& deleteAction = CreateWidget<Widget::MenuItem>("Delete");
	}

	virtual void Execute() override
	{
		if (m_widgets.size() > 0)
			ContextualMenu::Execute();
	}

	virtual void DeleteItem() = 0;

public:
	std::string filePath;
};

class FileContextualMenu : public BrowserItemContextualMenu
{
public:
	FileContextualMenu(const std::string& p_filePath) : BrowserItemContextualMenu(p_filePath) {}

	virtual void CreateList() override
	{
		BrowserItemContextualMenu::CreateList();
	}

	virtual void DeleteItem() override
	{
	}
};

class SceneContextualMenu : public FileContextualMenu
{
public:
	SceneContextualMenu(const std::string& p_filePath) : FileContextualMenu(p_filePath) {}

	virtual void CreateList() override
	{
		auto& editAction = CreateWidget<Widget::MenuItem>("Edit");

		editAction.ClickedEvent += [this]
		{
			EDITOR_EXEC(LoadSceneFromDisk(filePath));
		};

		FileContextualMenu::CreateList();
	}
};

Panel::AssetBrowser::AssetBrowser(const std::string& p_title) :
	PanelWindow(p_title)
{
	m_assetList = &CreateWidget<Widget::Group>();

	Fill();
}

void Panel::AssetBrowser::Fill()
{
	ConsiderItem(nullptr, std::filesystem::directory_entry("Contents"));
}

void Panel::AssetBrowser::Clear()
{
	m_assetList->RemoveAllWidgets();
}

void Panel::AssetBrowser::Refresh()
{
	Clear();
	Fill();
}

void Panel::AssetBrowser::ParseFolder(Widget::TreeNode& p_root, const std::filesystem::directory_entry& p_directory)
{
	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (item.is_directory())
			ConsiderItem(&p_root, item);

	for (auto& item : std::filesystem::directory_iterator(p_directory))
		if (!item.is_directory())
			ConsiderItem(&p_root, item);
}

void Panel::AssetBrowser::ConsiderItem(Widget::TreeNode* p_root, const std::filesystem::directory_entry& p_entry)
{
	bool isDirectory = p_entry.is_directory();

	std::string itemname = PathParser::GetElementName(p_entry.path().string());
	std::string path = p_entry.path().string();

	PathParser::EFileType fileType = PathParser::GetFileType(itemname);
	if (fileType == PathParser::EFileType::UNKNOWN && !isDirectory)
	{
		return;
	}

	auto& itemGroup = p_root ? p_root->CreateWidget<Widget::Group>() : m_assetList->CreateWidget<Widget::Group>();

	uint32_t iconTextureID = isDirectory ? EDITOR_CONTEXT(editorResource)->GetTexture("Icon_Folder")->id : EDITOR_CONTEXT(editorResource)->GetFileIcon(itemname)->id;
	itemGroup.CreateWidget<Widget::Image>(iconTextureID, FVector2{ 16, 16 }).lineBreak = false;

	if (isDirectory)
	{
		auto& treeNode = itemGroup.CreateWidget<Widget::TreeNode>(itemname);
		treeNode.OpenedEvent += [this, &treeNode, path]
		{
			treeNode.RemoveAllWidgets();
			std::string updatedPath = PathParser::GetContainingFolder(path) + treeNode.name;
			ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath));
		};
	}
	else
	{
		auto& clickableText = itemGroup.CreateWidget<Widget::TextClickable>(itemname);

		FileContextualMenu* contextMenu = nullptr;
		switch (fileType)
		{
		case PathParser::EFileType::SCENE: contextMenu = &clickableText.AddPlugin<SceneContextualMenu>(path); break;
		default: contextMenu = &clickableText.AddPlugin<FileContextualMenu>(path); break;
		}

		contextMenu->CreateList();

		clickableText.AddPlugin<DDSource<std::pair<std::string, Widget::Group*>>>
		(
			"File",
			itemname,
			std::make_pair(path, &itemGroup)
		);
	}
}
