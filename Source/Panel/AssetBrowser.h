#pragma once

#include <filesystem>

#include "PanelWindow.h"
#include "../Widget/TreeNode.h"
#include "../Widget/Group.h"

namespace Panel
{
	class AssetBrowser : public PanelWindow
	{
	public:
		AssetBrowser(const std::string& p_title);

		void Fill();
		void Clear();
		void Refresh();

	private:
		void ParseFolder(Widget::TreeNode& p_root, const std::filesystem::directory_entry& p_directory);
		void ConsiderItem(Widget::TreeNode* p_root, const std::filesystem::directory_entry& p_entry);

	private:
		Widget::Group* m_assetList;
	};
}
