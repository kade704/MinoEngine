#include "Scene.h"
#include "Serializer.h"
#include <algorithm>

Scene::Scene()
{
}

Scene::~Scene()
{
    std::for_each(m_actors.begin(), m_actors.end(), [](Actor* element)
    {
        delete element;
    });

    m_actors.clear();
}

void Scene::Play()
{
    m_isPlaying = true;

    std::for_each(m_actors.begin(), m_actors.end(), [](Actor* p_element) { p_element->OnStart(); });
}

void Scene::Update(float deltaTime)
{
    std::for_each(m_actors.begin(), m_actors.end(), std::bind(std::mem_fn(&Actor::OnUpdate), std::placeholders::_1, deltaTime));
}

bool Scene::IsPlaying() const
{
    return m_isPlaying;
}

Actor& Scene::CreateActor()
{
    return CreateActor("New Actor");
}

Actor& Scene::CreateActor(const std::string& name)
{
    m_actors.push_back(new Actor(m_availableID++, name));
    auto& instance = *m_actors.back();
    instance.ComponentAddedEvent += std::bind(&Scene::OnComponentAdded, this, std::placeholders::_1);
    instance.ComponentRemovedEvent += std::bind(&Scene::OnComponentRemoved, this, std::placeholders::_1);
    if (m_isPlaying)
    {
        instance.OnStart();
    }
    return instance;
}

void Scene::CollectGarbages()
{
    m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [this](Actor* element)
        {
            bool isGarbage = !element->IsAlive();
    if (isGarbage)
    {
        delete element;
    }
    return isGarbage;
        }), m_actors.end());
}

bool Scene::DestroyActor(Actor& target)
{
    auto found = std::find_if(m_actors.begin(), m_actors.end(), [&target](Actor* element)
        {
            return element == &target;
        });

    if (found != m_actors.end())
    {
        delete* found;
        m_actors.erase(found);
        return true;
    }
    else
    {
        return false;
    }
}

void Scene::OnComponentAdded(Component::AComponent& component)
{
    if (auto result = dynamic_cast<Component::CModelRenderer*>(&component))
        m_fastAccessComponents.modelRenderers.push_back(result);

    if (auto result = dynamic_cast<Component::CCamera*>(&component))
        m_fastAccessComponents.cameras.push_back(result);

    if (auto result = dynamic_cast<Component::CLight*>(&component))
        m_fastAccessComponents.lights.push_back(result);
}

void Scene::OnComponentRemoved(Component::AComponent& component)
{
    if (auto result = dynamic_cast<Component::CModelRenderer*>(&component))
        m_fastAccessComponents.modelRenderers.erase(std::remove(m_fastAccessComponents.modelRenderers.begin(), m_fastAccessComponents.modelRenderers.end(), result), m_fastAccessComponents.modelRenderers.end());

    if (auto result = dynamic_cast<Component::CCamera*>(&component))
        m_fastAccessComponents.cameras.erase(std::remove(m_fastAccessComponents.cameras.begin(), m_fastAccessComponents.cameras.end(), result), m_fastAccessComponents.cameras.end());

    if (auto result = dynamic_cast<Component::CLight*>(&component))
        m_fastAccessComponents.lights.erase(std::remove(m_fastAccessComponents.lights.begin(), m_fastAccessComponents.lights.end(), result), m_fastAccessComponents.lights.end());
}

Actor* Scene::FindActorByName(const std::string& name) const
{
    auto result = std::find_if(m_actors.begin(), m_actors.end(), [name](Actor* element)
        {
            return element->GetName() == name;
        });

    if (result != m_actors.end())
        return *result;
    else
        return nullptr;
}

Actor* Scene::FindActorByID(unsigned int id) const
{
    auto result = std::find_if(m_actors.begin(), m_actors.end(), [id](Actor* element)
        {
            return element->GetID() == id;
        });

    if (result != m_actors.end())
        return *result;
    else
        return nullptr;
}

Scene::FastAccessComponents Scene::GetFastAccessComponents() const
{
    return m_fastAccessComponents;
}

void Scene::OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
    auto actorsNode = doc.NewElement("actors");
    node->InsertEndChild(actorsNode);

    for (auto& actor : m_actors)
    {
        actor->OnSerialize(doc, actorsNode);
    }
}

void Scene::OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
    auto actorsRoot = node->FirstChildElement("actors");
    if (actorsRoot)
    {
        auto currentActor = actorsRoot->FirstChildElement("actor");

        while (currentActor)
        {
            auto& actor = CreateActor();
            actor.OnDeserialize(doc, currentActor);
            currentActor = currentActor->NextSiblingElement("actor");
        }

        for (auto actor : m_actors)
        {
            if (auto found = FindActorByID(actor->GetParentID()); found)
                actor->SetParent(*found);
        }
    }
}
