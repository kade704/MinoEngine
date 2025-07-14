#include "TransformNotifier.h"

TransformNotifier::NotificationHandlerID TransformNotifier::AddNotificationHandler(NotificationHandler notificationHandler)
{
	NotificationHandlerID handlerID = mAvailableHandlerID++;
	mNotificationHandlers.emplace(handlerID, notificationHandler);
	return handlerID;
}

void TransformNotifier::NotifyChildren(ENotification notification)
{
	if (!mNotificationHandlers.empty())
		for (auto const& [id, handler] : mNotificationHandlers)
			handler(notification);
}

bool TransformNotifier::RemoveNotificationHandler(NotificationHandlerID notificationHandlerID)
{
	return mNotificationHandlers.erase(notificationHandlerID) != 0;
}
