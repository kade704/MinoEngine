#include "Actor.h"
#include "Serializer.h"
#include "Component/CModelRenderer.h"
#include "Component/CMaterialRenderer.h"
#include "Component/CTransform.h"
#include "Component/CDirectionalLight.h"
#include "Component/CPointLight.h"
#include "Component/CSpotLight.h"
#include "Component/CCamera.h"
#include <algorithm>

Event<Actor&> Actor::DestroyedEvent;
Event<Actor&> Actor::CreatedEvent;
Event<Actor&, Actor&> Actor::AttachedEvent;
Event<Actor&> Actor::DetachedEvent;

Actor::Actor(int64_t p_actorID, const std::string& p_name) :
    m_name(p_name),
    m_actorID(p_actorID),
    m_parentID(-1),
    m_parent(nullptr),
    transform(AddComponent<Component::CTransform>())
{
    CreatedEvent.Invoke(*this);
}

Actor::~Actor()
{
    DestroyedEvent.Invoke(*this);

    std::vector<Actor*> toDetach = m_children;

    for (auto child : toDetach)
        child->DetachFromParent();

    toDetach.clear();

    DetachFromParent();

    std::for_each(m_components.begin(), m_components.end(), [&](std::shared_ptr<Component::AComponent> p_component) { ComponentRemovedEvent.Invoke(*p_component); });
    std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& p_behaviour) { BehaviourRemovedEvent.Invoke(std::ref(p_behaviour.second)); });
    std::for_each(m_children.begin(), m_children.end(), [](Actor* p_element) { delete p_element; });
}

const std::string& Actor::GetName() const
{
    return m_name;
}

void Actor::SetName(const std::string& name)
{
    m_name = name;
}

void Actor::SetID(int64_t p_id)
{
    m_actorID = p_id;
}

int64_t Actor::GetID() const
{
    return m_actorID;
}

int64_t Actor::GetParentID() const
{
    return m_parentID;
}

void Actor::OnStart()
{
    std::for_each(m_components.begin(), m_components.end(), [](auto element) { element->OnStart(); });
    std::for_each(m_behaviours.begin(), m_behaviours.end(), [](auto& element) { element.second.OnStart(); });
}

void Actor::OnUpdate(float p_deltaTime)
{
    std::for_each(m_components.begin(), m_components.end(), [&](auto element) { element->OnUpdate(p_deltaTime); });
    std::for_each(m_behaviours.begin(), m_behaviours.end(), [&](auto& element) { element.second.OnUpdate(p_deltaTime); });
}

bool Actor::RemoveComponent(Component::AComponent& component)
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        if (it->get() == &component)
        {
            ComponentRemovedEvent.Invoke(component);
            m_components.erase(it);
            return true;
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Component::AComponent>> Actor::GetComponents() const
{
    return m_components;
}

Component::Behaviour& Actor::AddBehaviour(const std::string& p_name)
{
    m_behaviours.try_emplace(p_name, *this, p_name);
    Component::Behaviour& newInstance = m_behaviours.at(p_name);
    BehaviourAddedEvent.Invoke(newInstance);

    return newInstance;
}

bool Actor::RemoveBehaviour(Component::Behaviour& p_behaviour)
{
    bool found = false;

    for (auto& [name, behaviour] : m_behaviours)
    {
        if (&behaviour == &p_behaviour)
        {
            found = true;
            break;
        }
    }

    if (found)
        return RemoveBehaviour(p_behaviour.name);
    else
        return false;
}

bool Actor::RemoveBehaviour(const std::string& p_name)
{
    Component::Behaviour* found = GetBehaviour(p_name);
    if (found)
    {
        BehaviourRemovedEvent.Invoke(*found);
        return m_behaviours.erase(p_name);
    }
    else
    {
        return false;
    }
}

Component::Behaviour* Actor::GetBehaviour(const std::string& p_name)
{
    if (auto result = m_behaviours.find(p_name); result != m_behaviours.end())
        return &result->second;
    else
        return nullptr;
}

std::unordered_map<std::string, Component::Behaviour>& Actor::GetBehaviours()
{
    return m_behaviours;
}

bool Actor::IsAlive() const
{
    return !m_destroyed;
}

void Actor::MarkAsDestroy()
{
    m_destroyed = true;

    for (auto child : m_children)
        child->MarkAsDestroy();
}

void Actor::DetachFromParent()
{
    DetachedEvent.Invoke(*this);

    if (m_parent)
    {
        m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](Actor* element)
        {
            return element == this;
        }));
    }

    m_parent = nullptr;
    m_parentID = 0;

    transform.RemoveParent();
}

void Actor::SetParent(Actor& parent)
{
    DetachFromParent();

    m_parent = &parent;
    m_parentID = parent.m_actorID;
    transform.SetParent(parent.transform);

    parent.m_children.push_back(this);

    AttachedEvent.Invoke(*this, parent);
}

bool Actor::HasParent() const
{
    return m_parent != nullptr;
}

Actor* Actor::GetParent() const
{
    return m_parent;
}

const std::vector<Actor*>& Actor::GetChildren() const
{
    return m_children;
}

void Actor::OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
    auto actorNode = doc.NewElement("actor");
    node->InsertEndChild(actorNode);

    Serializer::SerializeString(doc, actorNode, "name", m_name);
    Serializer::SerializeInt64(doc, actorNode, "id", m_actorID);
    Serializer::SerializeInt64(doc, actorNode, "parent", m_parentID);

    auto componentsNode = doc.NewElement("components");
    actorNode->InsertEndChild(componentsNode);

    for (auto& component : m_components)
    {
        auto componentNode = doc.NewElement("component");
        componentsNode->InsertEndChild(componentNode);

        Serializer::SerializeString(doc, componentNode, "type", component->GetName());

        auto data = doc.NewElement("data");
        componentNode->InsertEndChild(data);

        component->OnSerialize(doc, data);
    }
}

void Actor::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot)
{
    Serializer::DeserializeString(p_doc, p_actorsRoot, "name", m_name);
    Serializer::DeserializeInt64(p_doc, p_actorsRoot, "id", m_actorID);
    Serializer::DeserializeInt64(p_doc, p_actorsRoot, "parent", m_parentID);

    auto componentsRoot = p_actorsRoot->FirstChildElement("components");
    if (componentsRoot)
    {
        auto currentComponent = componentsRoot->FirstChildElement("component");

        while (currentComponent)
        {
            std::string componentType = currentComponent->FirstChildElement("type")->GetText();
            Component::AComponent* component = nullptr;

            if (componentType == Component::CTransform::GetNameStatic()) component = &transform;
            else if (componentType == Component::CModelRenderer::GetNameStatic()) component = &AddComponent<Component::CModelRenderer>();
            else if (componentType == Component::CMaterialRenderer::GetNameStatic()) component = &AddComponent<Component::CMaterialRenderer>();
            else if (componentType == Component::CDirectionalLight::GetNameStatic()) component = &AddComponent<Component::CDirectionalLight>();
            else if (componentType == Component::CPointLight::GetNameStatic()) component = &AddComponent<Component::CPointLight>();
            else if (componentType == Component::CSpotLight::GetNameStatic()) component = &AddComponent<Component::CSpotLight>();
            else if (componentType == Component::CCamera::GetNameStatic()) component = &AddComponent<Component::CCamera>();
            else
            {
                MINO_ERROR("Unknown component type: " + componentType);
                currentComponent = currentComponent->NextSiblingElement("component");
                continue;
            }
            if (component)
                component->OnDeserialize(p_doc, currentComponent->FirstChildElement("data"));

            currentComponent = currentComponent->NextSiblingElement("component");
        }
    }

    tinyxml2::XMLNode* behavioursRoot = p_actorsRoot->FirstChildElement("behaviours");
    if (behavioursRoot)
    {
        tinyxml2::XMLElement* currentBehaviour = behavioursRoot->FirstChildElement("behaviour");
        while (currentBehaviour)
        {
            std::string behaviourType = currentBehaviour->FirstChildElement("type")->GetText();

            auto& behaviour = AddBehaviour(behaviourType);
            behaviour.OnDeserialize(p_doc, currentBehaviour->FirstChildElement("data"));
            
            currentBehaviour = currentBehaviour->NextSiblingElement("behaviour");
        }
    }
}