#include "renderer.hpp"
#include <algorithm>

static RenderState render_state;

RenderState& get_render_state()
{
	return render_state;
}

void render_background()
{
	auto pixel = (uint32_t*)render_state.buffer;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = x * y;
		}
	}
}


void clear_screen(uint32_t color)
{
	auto pixel = (uint32_t*)render_state.buffer;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

void draw_rect_pixels(int x0, int y0, int x1, int y1, uint32_t color)
{
	x0 = std::clamp(x0, 0, render_state.width);
	x1 = std::clamp(x1, 0, render_state.width);
	y0 = std::clamp(y0, 0, render_state.height);
	y1 = std::clamp(y1, 0, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		auto pixel = (uint32_t*)render_state.buffer + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

static float render_scale = 0.01f;

void draw_rect(float x, float y, float half_size_x, float half_size_y, uint32_t color)
{
// X11 draws bottom to top
#ifdef __linux__
	y = -y;
#endif

	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_pixels(x0, y0, x1, y1, color);
}

void draw_number(int number, float x, float y, float size, uint32_t color)
{
	float half_size = size * .5f;

	do
	{
		int digit = number % 10;
		number = number / 10;
		switch (digit)
		{
		case 0:
		{
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			break;
		}
		case 1:
		{
			draw_rect(x, y, half_size, size * 2.5f, color);
			break;
		}
		case 2:
		{
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x + size, y + size, half_size, half_size, color);
			draw_rect(x - size, y - size, half_size, half_size, color);
			break;
		}
		case 3:
		{
			draw_rect(x, y + size * 2.f, half_size * 2.5f, half_size, color);
			draw_rect(x, y - size * 2.f, half_size * 2.5f, half_size, color);
			draw_rect(x, y, half_size * 2.5f, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			break;
		}
		case 4: 
		{
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, 1.5f * size, color);
			draw_rect(x, y, half_size, half_size, color);
			break;
		}
		case 5:
		{
			draw_rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x, y, 1.5f * size, half_size, color);
			draw_rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			break;
		}
		case 6:
		{
			draw_rect(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x + half_size, y, size, half_size, color);
			draw_rect(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y - size, half_size, half_size, color);
			break;
		}
		case 7:
		{
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			break;
		}
		case 8:
		{
			draw_rect(x - size, y, half_size, 2.5f * size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x, y - size * 2.f, half_size, half_size, color);
			draw_rect(x, y + size * 2.f, half_size, half_size, color);
			draw_rect(x, y, half_size, half_size, color);
			break;
		}
		case 9:
		{
			draw_rect(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rect(x - half_size, y, size, half_size, color);
			draw_rect(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rect(x + size, y, half_size, 2.5f * size, color);
			draw_rect(x - size, y + size, half_size, half_size, color);
			break;
		}
		}
		x -= size * 4.f;
	} while (number);
}