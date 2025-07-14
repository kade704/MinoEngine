#pragma once

#include <unordered_map>

template<typename T>
class AResourceManager
{
public:
	T* LoadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
			return resource;
		else
		{
			auto newResource = CreateResource(p_path);
			if (newResource)
				return RegisterResource(p_path, newResource);
			else
				return nullptr;
		}
	}

	void UnloadResource(const std::string& p_path)
	{
		if (auto resource = GetResource(p_path, false); resource)
		{
			DestroyResource(resource);
			UnregisterResource(p_path);
		}
	}

	T* GetResource(const std::string& p_path, bool p_tryToLoadIfNotFound = true)
	{
		if (auto resource = m_resources.find(p_path); resource != m_resources.end())
		{
			return resource->second;
		}
		else if (p_tryToLoadIfNotFound)
		{
			return LoadResource(p_path);
		}

		return nullptr;
	}

	T* RegisterResource(const std::string& p_path, T* p_instance)
	{
		if (auto resource = GetResource(p_path, false); resource)
			DestroyResource(resource);

		m_resources[p_path] = p_instance;

		return p_instance;
	}

	void UnregisterResource(const std::string& p_path)
	{
		m_resources.erase(p_path);
	}

protected:
	virtual T* CreateResource(const std::string& p_path) = 0;
	virtual void DestroyResource(T* p_resource) = 0;

private:
	std::unordered_map<std::string, T*> m_resources;
};