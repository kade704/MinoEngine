#pragma once

#include <vector>
#include <string>
#include <memory>

#include <Component/AComponent.h>
#include <Component/CTransform.h>
#include <Component/Behaviour.h>
#include <Event.h>
#include <Math.h>
#include <Serializable.h>
#include <Component/Behaviour.h>

class Actor : public Serializable
{
public:
	Actor(int64_t id, const std::string& name);
	virtual ~Actor();

	const std::string& GetName() const;
	void SetName(const std::string& name);

	int64_t GetID() const;
	int64_t GetParentID() const;

	void OnStart();
	void OnUpdate(float p_deltaTime);

	bool RemoveComponent(Component::AComponent& component);

	template<typename T>
	bool RemoveComponent()
	{
		static_assert(std::is_base_of<Component::AComponent, T>::value, "T should derive from Component");
		static_assert(!std::is_same<Component::CTransform, T>::value, "You can't remove a Transform from an actor");

		std::shared_ptr<T> result(nullptr);
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			result = std::dynamic_pointer_cast<T>(*it);
			if (result)
			{
				ComponentRemovedEvent.Invoke(*result.get());
				m_components.erase(it);

				return true;
			}
		}

		return false;
	}

	const std::vector<std::shared_ptr<Component::AComponent>> GetComponents() const;

	template<typename T, typename ... Args>
	T& AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component::AComponent, T>::value, "T should derive from Component");

		if (auto found = GetComponent<T>(); !found)
		{
			m_components.insert(m_components.begin(), std::make_shared<T>(*this, args...));
			T& instance = *dynamic_cast<T*>(m_components.front().get());
			ComponentAddedEvent.Invoke(instance);
			return instance;
		}
		else
		{
			return *found;
		}
	}

	template<typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<Component::AComponent, T>::value, "T should derive from Component");

		std::shared_ptr<T> result(nullptr);
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			result = std::dynamic_pointer_cast<T>(*it);
			if (result)
			{
				return result.get();
			}
		}

		return nullptr;
	}

	Component::Behaviour& AddBehaviour(const std::string& p_name);
	bool RemoveBehaviour(Component::Behaviour& p_behaviour);
	bool RemoveBehaviour(const std::string& p_name); 
	Component::Behaviour* GetBehaviour(const std::string& p_name);
	std::unordered_map<std::string, Component::Behaviour>& GetBehaviours();

	bool IsAlive() const;
	void MarkAsDestroy();
	void DetachFromParent();
	void SetParent(Actor& parent);
	bool HasParent() const;
	Actor* GetParent() const;
	const std::vector<Actor*>& GetChildren() const;

	void OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;
	void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot) override;
		
public:
	Event<Component::AComponent&> ComponentAddedEvent;
	Event<Component::AComponent&> ComponentRemovedEvent;
	Event<Component::Behaviour&> BehaviourAddedEvent;
	Event<Component::Behaviour&> BehaviourRemovedEvent;

	static Event<Actor&> DestroyedEvent;
	static Event<Actor&> CreatedEvent;
	static Event<Actor&, Actor&> AttachedEvent;
	static Event<Actor&> DetachedEvent;

private:
	std::string m_name;
	int64_t m_actorID;
	int64_t m_parentID;
	Actor* m_parent;
	std::vector<Actor*> m_children;
	std::vector<std::shared_ptr<Component::AComponent>> m_components;
	std::unordered_map<std::string, Component::Behaviour> m_behaviours;
	bool m_destroyed = false;
	bool m_playing = false;

public:
	Component::CTransform& transform;
};