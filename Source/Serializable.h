#pragma once

#include "tinyxml2.h"

class Serializable
{
public:
	virtual ~Serializable() = default;
	virtual void OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) = 0;
	virtual void OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node) = 0;
};
