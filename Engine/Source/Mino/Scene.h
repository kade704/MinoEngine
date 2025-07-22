#pragma once

#include <vector>
#include <string>

#include "Actor.h"
#include "Component/CModelRenderer.h"
#include "Component/CCamera.h"
#include "Component/CLight.h"
#include "Event.h"
#include "Serializable.h"

class Scene : public Serializable
{
public:
	struct FastAccessComponents
	{
		std::vector<Component::CModelRenderer*> modelRenderers;
		std::vector<Component::CCamera*> cameras;
		std::vector<Component::CLight*> lights;
	};

	static Scene* Create(const std::string& p_path);

	Scene();
	~Scene();

	void Play();
	void Update(float p_deltaTima);

	bool IsPlaying() const;

	Actor& CreateActor();
	Actor& CreateActor(const std::string& p_name);

	void CollectGarbages();

	bool DestroyActor(Actor& target);

	void OnComponentAdded(Component::AComponent& component);
	void OnComponentRemoved(Component::AComponent& component);

	Actor* FindActorByName(const std::string& name) const;
	Actor* FindActorByID(unsigned int id) const;

	FastAccessComponents GetFastAccessComponents() const;

	virtual void OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;
	virtual void OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) override;

private:
	int64_t m_availableID = 1;
	bool m_isPlaying = false;
	std::vector<Actor*> m_actors;
	FastAccessComponents m_fastAccessComponents;
};

