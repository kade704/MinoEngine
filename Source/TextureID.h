#pragma once

#include <cstdint>

union TextureID
{
	uint32_t id;
	void* raw;
};