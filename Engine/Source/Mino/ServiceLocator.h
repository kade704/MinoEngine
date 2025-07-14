#pragma once
#include <unordered_map>
#include <any>

#define MINO_SERVICE(Type) ServiceLocator::Get<Type>()

class ServiceLocator
{
public:
	template<typename T>
	static void Provide(T& p_service);

	template<typename T>
	static T& Get();

private:
	static std::unordered_map<size_t, std::any> __SERVICES;
};

template<typename T>
static void ServiceLocator::Provide(T& p_service)
{
	__SERVICES[typeid(T).hash_code()] = std::any(&p_service);
}

template<typename T>
static T& ServiceLocator::Get()
{
	return *std::any_cast<T*>(__SERVICES[typeid(T).hash_code()]);
}