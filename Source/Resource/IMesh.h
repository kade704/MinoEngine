#pragma once

#include <vector>

class IMesh
{
public:
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual uint32_t GetVertexCount() = 0;
	virtual uint32_t GetIndexCount() = 0;
};