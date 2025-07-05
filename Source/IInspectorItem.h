#pragma once

#include "Serializable.h"
#include "WidgetContainer.h"

class IInspectorItem : public Serializable
{
public:
	virtual void OnInspector(WidgetContainer& p_root) = 0;
};