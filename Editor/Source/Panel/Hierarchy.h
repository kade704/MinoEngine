#pragma once

#include <unordered_map>

#include <Mino/Panel/PanelWindow.h>
#include <Mino/Event.h>
#include <Mino/Actor.h>
#include <Mino/Widget/TreeNode.h>
#include <Mino/ContextualMenu.h>

namespace Panel
{
	class Hierarchy : public PanelWindow
	{
	public:
		Hierarchy(const std::string& p_title);

		void UnselectActorsWidgets();
		void SelectActorByInstance(Actor& p_actor);
		void SelectActorByWidget(Widget::TreeNode& p_widget);

		void AttachActorToParent(Actor& p_actor);
		void DetachFromParent(Actor& p_actor);

		void DeleteActorByInstance(Actor& p_actor);
		void AddActorByInstance(Actor& p_actor);

		void ExpandTreeNode(Widget::TreeNode& p_toExpand);

	public:
		Event<Actor&> ActorSelectedEvent;
		Event<Actor&> ActorDeselectedEvent;

	private:
		Widget::TreeNode* m_sceneRoot;
		std::unordered_map<Actor*, Widget::TreeNode*> m_widgetActorLink;
	};
}
