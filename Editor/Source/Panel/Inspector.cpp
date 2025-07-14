#include <map>

#include "Inspector.h"
#include "../Context.h"
#include "../EditorAction.h"

#include <Mino/IInspectorItem.h>
#include <Mino/GUIDrawer.h>
#include <Mino/Widget/Column.h>
#include <Mino/Widget/GroupCollapsable.h>
#include <Mino/Widget/Button.h>
#include <Mino/Component/CMaterialRenderer.h>
#include <Mino/Widget/Separator.h>
#include <Mino/Component/CDirectionalLight.h>
#include <Mino/Component/CPointLight.h>
#include <Mino/Component/CSpotLight.h>
#include <Mino/Component/CModelRenderer.h>
#include <Mino/Component/CTransform.h>

Panel::Inspector::Inspector(const std::string& p_title) :
    PanelWindow(p_title)
{
    m_inspectorHeader = &CreateWidget<Widget::Group>();
    m_inspectorHeader->enabled = false;

    m_actorInfo = &CreateWidget<Widget::Group>();

    auto& headerColumns = m_inspectorHeader->CreateWidget<Widget::Column<2>>();

    auto nameGatherer = [this] { return m_targetActor ? m_targetActor->GetName() : "%undef%"; };
    auto nameProvider = [this](const std::string& p_newName) { if (m_targetActor) m_targetActor->SetName(p_newName); };
    GUIDrawer::DrawString(headerColumns, "Name", nameGatherer, nameProvider);

    auto& componentSelectorWidget = m_inspectorHeader->CreateWidget<Widget::ComboBox>(0);
    componentSelectorWidget.lineBreak = false;
    componentSelectorWidget.choices.emplace(0, "Model Renderer");
    componentSelectorWidget.choices.emplace(1, "Material Renderer");
    componentSelectorWidget.choices.emplace(2, "Directional Light");
    componentSelectorWidget.choices.emplace(3, "Point Light");
    componentSelectorWidget.choices.emplace(4, "Spot Light");

    auto& addComponentButton = m_inspectorHeader->CreateWidget<Widget::Button>("Add Component", FVector2{ 100.f, 0 });
    addComponentButton.idleBackgroundColor = Color{ 0.7f, 0.5f, 0.f };
    addComponentButton.textColor = Color::White;
    addComponentButton.ClickedEvent += [&componentSelectorWidget, this]
    {
        switch (componentSelectorWidget.currentChoice)
        {
        case 0: GetTargetActor()->AddComponent<Component::CModelRenderer>(); break;
        case 1: GetTargetActor()->AddComponent<Component::CMaterialRenderer>(); break;
        case 2: GetTargetActor()->AddComponent<Component::CDirectionalLight>(); break;
        case 3: GetTargetActor()->AddComponent<Component::CPointLight>(); break;
        case 4: GetTargetActor()->AddComponent<Component::CSpotLight>(); break;
        }

        componentSelectorWidget.ValueChangedEvent.Invoke(componentSelectorWidget.currentChoice);
        componentSelectorWidget.currentChoice = 0;
        Refresh();
    };

    componentSelectorWidget.ValueChangedEvent += [this, &addComponentButton](int p_value)
    {
        auto defineButtonsStates = [&addComponentButton](bool p_componentExists)
        {
            addComponentButton.disabled = p_componentExists;
            addComponentButton.idleBackgroundColor = !p_componentExists ? Color{ 0.7f, 0.5f, 0.f } : Color{ 0.1f, 0.1f, 0.1f };
        };

        switch (p_value)
        {
        case 0: defineButtonsStates(GetTargetActor()->GetComponent<Component::CModelRenderer>()); return;
        case 1: defineButtonsStates(GetTargetActor()->GetComponent<Component::CMaterialRenderer>()); return;
        }
    };

    m_componentSelectorWidget = &componentSelectorWidget;

    m_inspectorHeader->CreateWidget<Widget::Separator>();

    m_destroyedListener = Actor::DestroyedEvent += [this](Actor& p_destroyed)
    {
        if (&p_destroyed == m_targetActor)
            Unfocus();
    };
}

Panel::Inspector::~Inspector()
{
    Actor::DestroyedEvent -= m_destroyedListener;
}

void Panel::Inspector::FocusActor(Actor& p_target)
{
    if (m_targetActor)
        Unfocus();

    m_actorInfo->RemoveAllWidgets();

    m_targetActor = &p_target;

    m_componentAddedListener = m_targetActor->ComponentAddedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };
    m_componentRemovedListener = m_targetActor->ComponentRemovedEvent += [this](auto& useless) { EDITOR_EXEC(DelayAction([this] { Refresh(); })); };

    m_inspectorHeader->enabled = true;

    CreateActorInspector(p_target);

    EDITOR_EVENT(ActorSelectedEvent).Invoke(*m_targetActor);
}

void Panel::Inspector::Unfocus()
{
    if (!m_targetActor)
    {
        return;
    }

    m_targetActor->ComponentAddedEvent -= m_componentAddedListener;
    m_targetActor->ComponentRemovedEvent -= m_componentRemovedListener;

    EDITOR_EVENT(ActorUnselectedEvent).Invoke(*m_targetActor);

    SoftUnfocus();
}

void Panel::Inspector::SoftUnfocus()
{
    if (m_targetActor)
    {
        m_inspectorHeader->enabled = false;
        m_targetActor = nullptr;
        m_actorInfo->RemoveAllWidgets();
    }
}

Actor* Panel::Inspector::GetTargetActor() const
{
    return m_targetActor;
}

void Panel::Inspector::Refresh()
{
    if (m_targetActor)
    {
        m_actorInfo->RemoveAllWidgets();
        CreateActorInspector(*m_targetActor);
    }
}

void Panel::Inspector::CreateActorInspector(Actor& p_target)
{
    std::map<std::string, Component::AComponent*> components;

    for (auto component : p_target.GetComponents())
        if (component->GetName() != "Transform")
            components[component->GetName()] = component.get();

    auto transform = p_target.GetComponent<Component::CTransform>();
    if (transform)
        DrawComponent(*transform);

    for (auto& [name, instance] : components)
        DrawComponent(*instance);

    auto& behaviours = p_target.GetBehaviours();

    for (auto& [name, behaviour] : behaviours)
        DrawBehaviour(behaviour);
}

void Panel::Inspector::DrawComponent(Component::AComponent& p_component)
{
    if (auto inspectorItem = dynamic_cast<IInspectorItem*>(&p_component); inspectorItem)
    {
        auto& header = m_actorInfo->CreateWidget<Widget::GroupCollapsable>(p_component.GetName());
        header.closable = !dynamic_cast<Component::CTransform*>(&p_component);
        header.CloseEvent += [this, &header, &p_component]
        {
            if(p_component.owner.RemoveComponent(p_component))
                m_componentSelectorWidget->ValueChangedEvent.Invoke(m_componentSelectorWidget->currentChoice);
        };
        auto& columns = header.CreateWidget<Widget::Column<2>>();
        columns.widths[0] = 150;
        inspectorItem->OnInspector(columns);
    }
}

void Panel::Inspector::DrawBehaviour(Component::Behaviour& p_behaviour)
{
    if (auto inspectorItem = dynamic_cast<IInspectorItem*>(&p_behaviour); inspectorItem)
    {
        auto& header = m_actorInfo->CreateWidget<Widget::GroupCollapsable>(p_behaviour.name);
        header.closable = true;
        header.CloseEvent += [this, &header, &p_behaviour]
        {
            p_behaviour.owner.RemoveBehaviour(p_behaviour);
        };

        auto& columns = header.CreateWidget<Widget::Column<2>>();
        columns.widths[0] = 200;
        inspectorItem->OnInspector(columns);
    }
}

