#include "AssetBrowser.h"
#include "../EditorAction.h"

#include <Mino/PathParser.h>
#include <Mino/Widget/TextClickable.h>
#include <Mino/Widget/Image.h>
#include <Mino/DDSource.h>
#include <Mino/ContextualMenu.h>
#include <Mino/Widget/MenuList.h>
#include <Mino/Widget/MenuItem.h>
#include <Mino/Widget/InputText.h>

#include <fstream>

class BrowserItemContextualMenu : public ContextualMenu
{
public:
	BrowserItemContextualMenu(const std::string p_filePath) : filePath(p_filePath) {}

	virtual void CreateList()
	{
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

class FolderContextualMenu : public BrowserItemContextualMenu
{
public:
	FolderContextualMenu(const std::string& p_filePath) : BrowserItemContextualMenu(p_filePath) {}

	virtual void CreateList() override
	{
		auto& createMenu = CreateWidget<Widget::MenuList>("Create");

		auto& createFolderMenu = createMenu.CreateWidget<Widget::MenuList>("Folder");
		auto& createMaterialMenu = createMenu.CreateWidget<Widget::MenuList>("Material");
		auto& createFolderInput = createFolderMenu.CreateWidget<Widget::InputText>("");
		auto& createMaterialInput = createMaterialMenu.CreateWidget<Widget::InputText>("");

		createFolderMenu.ClickedEvent += [&createFolderInput] { createFolderInput.content = ""; };
		createMaterialMenu.ClickedEvent += [&createMaterialInput] { createMaterialInput.content = ""; };

		createFolderInput.EnterPressedEvent += [this](std::string newFolderName)
		{
			size_t fails = 0;
			std::string finalPath;

			do
			{
				finalPath = filePath + "/" + (!fails ? newFolderName : newFolderName + " (" + std::to_string(fails) + ')');

				++fails;
			} while (std::filesystem::exists(finalPath));

			std::filesystem::create_directory(finalPath);

			ItemAddedEvent.Invoke(finalPath);
			Close();
		};

		createMaterialInput.EnterPressedEvent += [this](std::string materialName)
		{
			size_t fails = 0;
			std::string finalPath;

			do
			{
				finalPath = filePath + "/" +  (!fails ? materialName : materialName + " (" + std::to_string(fails) + ')') + ".mat";

				++fails;
			} while (std::filesystem::exists(finalPath));

			{
				std::ofstream outfile(finalPath);
				outfile << "<root><shader>Resource/Shader/standard.shader</shader></root>" << std::endl;
			}

			ItemAddedEvent.Invoke(finalPath);


			// if (auto instance = EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))])
			// {
			// 	auto& materialEditor = EDITOR_PANEL(OvEditor::Panels::MaterialEditor, "Material Editor");
			// 	materialEditor.SetTarget(*instance);
			// 	materialEditor.Open();
			// 	materialEditor.Focus();
			// 	materialEditor.Preview();
			// }
			Close();
		};

		BrowserItemContextualMenu::CreateList();
	}

	virtual void DeleteItem() override
	{
	}

public:
	Event<std::string> ItemAddedEvent;
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
		auto& editAction = CreateWidget<Widget::MenuItem>("Open Scene");

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

		auto& contextMenu = treeNode.AddPlugin<FolderContextualMenu>(path);

		contextMenu.ItemAddedEvent += [this, &treeNode, path] (std::string p_string)
		{
			treeNode.Open();
			treeNode.RemoveAllWidgets();
			ParseFolder(treeNode, std::filesystem::directory_entry(PathParser::GetContainingFolder(p_string)));
		};
		
		contextMenu.CreateList();

		treeNode.OpenedEvent += [this, &treeNode, path]
	{
			treeNode.RemoveAllWidgets();
			std::string updatedPath = PathParser::GetContainingFolder(path) + treeNode.name;
			ParseFolder(treeNode, std::filesystem::directory_entry(updatedPath));
		};

		treeNode.ClosedEvent += [this, &treeNode]
		{
			treeNode.RemoveAllWidgets();
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
