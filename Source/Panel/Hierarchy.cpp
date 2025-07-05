#include "Hierarchy.h"
#include "../EditorAction.h"
#include "../imgui/imgui.h"
#include "../Widget/MenuItem.h"
#include "../DDSource.h"
#include "../DDTarget.h"

class HierarchyContextualMenu : public ContextualMenu
{
public:
	HierarchyContextualMenu(Actor* p_target, Widget::TreeNode& p_treeNode, bool p_panelMenu = false) :
		m_target(p_target),
		m_treeNode(p_treeNode)
	{
		auto& createActor = CreateWidget<Widget::MenuItem>("Create");
		createActor.ClickedEvent += [p_target] {
			EDITOR_EXEC(CreateActor(p_target, "New Actor"));
		};

		if (m_target)
		{
			auto& deleteButton = CreateWidget<Widget::MenuItem>("Delete");
			deleteButton.ClickedEvent += [this]
			{
				EDITOR_EXEC(DestroyActor(std::ref(*m_target)));
			};
		}
	}

	virtual void Execute() override
	{
		if (m_widgets.size() > 0)
			ContextualMenu::Execute();
	}

private:
	Actor* m_target;
	Widget::TreeNode& m_treeNode;
};

Panel::Hierarchy::Hierarchy(const std::string& p_title) :
	PanelWindow(p_title)
{
	m_sceneRoot = &CreateWidget<Widget::TreeNode>("Root");
	m_sceneRoot->Open();
	m_sceneRoot->AddPlugin<DDTarget<std::pair<Actor*, Widget::TreeNode*>>>("Actor").DataReceivedEvent += [this](std::pair<Actor*, Widget::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		m_sceneRoot->ConsiderWidget(*p_element.second);

		p_element.first->DetachFromParent();
	};
	m_sceneRoot->AddPlugin<HierarchyContextualMenu>(nullptr, *m_sceneRoot);

	Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	Actor::DestroyedEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	Actor::AttachedEvent += std::bind(&Hierarchy::AttachActorToParent, this, std::placeholders::_1);
	Actor::DetachedEvent += std::bind(&Hierarchy::DetachFromParent, this, std::placeholders::_1);
}

void Panel::Hierarchy::UnselectActorsWidgets()
{
	for (auto& widget : m_widgetActorLink)
		widget.second->selected = false;
}

void Panel::Hierarchy::SelectActorByInstance(Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
		if (result->second)
			SelectActorByWidget(*result->second);
}

void Panel::Hierarchy::SelectActorByWidget(Widget::TreeNode& p_widget)
{
	UnselectActorsWidgets();

	p_widget.selected = true;
}

void Panel::Hierarchy::AttachActorToParent(Actor& p_actor)
{
	auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		if (p_actor.HasParent())
		{
			auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->leaf = false;
			parentWidget->ConsiderWidget(*widget);
		}
	}
}

void Panel::Hierarchy::DetachFromParent(Actor& p_actor)
{
	if (auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_actor.HasParent() && p_actor.GetParent()->GetChildren().size() == 1)
		{
			if (auto parentWidget = m_widgetActorLink.find(p_actor.GetParent()); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->leaf = true;
			}
		}

		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		m_sceneRoot->ConsiderWidget(*widget);
	}
}

void Panel::Hierarchy::DeleteActorByInstance(Actor& p_actor)
{
	if (auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			result->second->Destroy();
		}

		m_widgetActorLink.erase(result);
	}
}

void Panel::Hierarchy::AddActorByInstance(Actor& p_actor)
{
	auto& textSelectable = m_sceneRoot->CreateWidget<Widget::TreeNode>(p_actor.GetName(), true);
	textSelectable.leaf = true;
	textSelectable.AddPlugin<HierarchyContextualMenu>(&p_actor, textSelectable);
	textSelectable.AddPlugin<DDSource<std::pair<Actor*, Widget::TreeNode*>>>("Actor", "Attach to...", std::make_pair(&p_actor, &textSelectable));
	textSelectable.AddPlugin<DDTarget<std::pair<Actor*, Widget::TreeNode*>>>("Actor").DataReceivedEvent += [&p_actor, &textSelectable](std::pair<Actor*, Widget::TreeNode*> p_element)
	{
		if (p_element.second->HasParent())
			p_element.second->GetParent()->UnconsiderWidget(*p_element.second);

		textSelectable.ConsiderWidget(*p_element.second);

		p_element.first->SetParent(p_actor);
	};

	auto& dispatcher = textSelectable.AddPlugin<DataDispatcher<std::string>>();

	Actor* targetPtr = &p_actor;
	dispatcher.RegisterGatherer([targetPtr] { return targetPtr->GetName(); });

	m_widgetActorLink[targetPtr] = &textSelectable;

	textSelectable.ClickedEvent += EDITOR_BIND(SelectActor, std::ref(p_actor));
}
