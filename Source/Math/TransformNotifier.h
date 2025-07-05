#pragma once
#include <functional>
#include <unordered_map>

class TransformNotifier
{
public:
	enum class ENotification
	{
		TRANSFORM_CHANGED,
		TRANSFORM_DESTROYED
	};

	using NotificationHandler = std::function<void(ENotification)>;
	using NotificationHandlerID = uint64_t;

	NotificationHandlerID AddNotificationHandler(NotificationHandler notificationHandler);
	void NotifyChildren(ENotification notification);

	bool RemoveNotificationHandler(NotificationHandlerID notificationHandlerID);

private:
	std::unordered_map<NotificationHandlerID, NotificationHandler> mNotificationHandlers;
	NotificationHandlerID mAvailableHandlerID = 0;
};

