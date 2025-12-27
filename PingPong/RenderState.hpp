#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

struct RenderState
{
	int width, height;
	// uint32 _RGB array
	void* buffer;
#ifdef _WIN32
	BITMAPINFO bitmap_info;
#endif
};