#pragma once

#include <functional>
#include <unordered_map>

using ListenerID = unsigned int;

template<class... ArgTypes>
class Event
{
public:
	using Callback = std::function<void(ArgTypes...)>;

	ListenerID AddListener(Callback callback)
	{
		ListenerID listenerID = mAvailableListenerID++;
		mCallbacks.emplace(listenerID, callback);
		return listenerID;
	}

	void RemoveListener(ListenerID listenerID)
	{
		mCallbacks.erase(listenerID);
	}

	ListenerID operator+=(Callback callback)
	{
		return AddListener(callback);
	}

	void operator-=(ListenerID listenerID)
	{
		RemoveListener(listenerID);
	}

	void RemoveAllListeners()
	{
		mCallbacks.clear();
	}

	void Invoke(ArgTypes... args)
	{
		for (auto const& [key, value] : mCallbacks)
			value(args...);
	}

private:
	std::unordered_map<ListenerID, Callback> mCallbacks;
	ListenerID mAvailableListenerID = 0;
};