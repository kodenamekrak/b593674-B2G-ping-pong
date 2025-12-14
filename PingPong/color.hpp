#pragma once

#include <cstdint>

struct Color
{
	uint8_t r, g, b;

	Color(uint8_t r, uint8_t g, uint8_t b): r(r), g(g), b(b)
	{}

	// returns u32 intended for XRGB8888 memory layout
	operator uint32_t() const
	{
		return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (b);
	}

};