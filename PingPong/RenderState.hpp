#pragma once

#include <windows.h>

struct RenderState
{
	int width, height;
	// uint32 _RGB array
	void* buffer;
	BITMAPINFO bitmap_info;
};